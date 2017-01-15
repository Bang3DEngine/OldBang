#include "BehaviourManager.h"

#include <QLibrary>

#include "Debug.h"
#include "Project.h"
#include "Persistence.h"
#include "Application.h"
#include "SystemUtils.h"
#include "ProjectManager.h"
#include "BehaviourHolder.h"
#include "BehaviourManagerCompileThread.h"

BehaviourManager::BehaviourManager()
{
    QObject::connect(&m_checkCompiledBehavioursTimer, SIGNAL(timeout()),
                     this, SLOT(TreatCompiledBehaviours()) );
    m_checkCompiledBehavioursTimer.start(1000);
}

BehaviourManager *BehaviourManager::GetInstance()
{
    Application *app = Application::GetInstance();
    return app ? app->m_behaviourManager : nullptr;
}

// Called by the BehaviourManagerCompileThread when has finished
void BehaviourManager::OnBehaviourFinishedCompiling(const String &behaviourPath,
                                                    const String &libraryFilepath)
{
    QMutexLocker locker(&m_mutex);
    String absPath = Persistence::ToAbsolute(behaviourPath, false);
    m_behPathsBeingCompiled.erase(absPath);
    m_behFilepath_To_compiledLibrary.Set(behaviourPath, libraryFilepath);
}

void BehaviourManager::OnBehaviourFailedCompiling(const String &behaviourPath)
{
    QMutexLocker locker(&m_mutex);
    String absPath = Persistence::ToAbsolute(behaviourPath, false);
    m_behPathsBeingCompiled.erase(absPath);
    m_failed_behFilepath_To_behaviourHash.Set(behaviourPath,
                                              Persistence::GetHash(absPath));
}

void BehaviourManager::TreatCompiledBehaviours()
{
    QMutexLocker locker(&m_mutex);

    for (auto it = m_behFilepath_To_compiledLibrary.Begin();
         it != m_behFilepath_To_compiledLibrary.End(); ++it)
    {
        String behaviourPath = it->first;
        String absPath = Persistence::ToAbsolute(behaviourPath, false);
        String hash = Persistence::GetHash(absPath);

        BehaviourManager *bm = BehaviourManager::GetInstance();

        // Open the library
        String libraryFilepath = it->second;
        QLibrary *lib = new  QLibrary(libraryFilepath.ToCString());
        lib->setLoadHints(QLibrary::LoadHint::ResolveAllSymbolsHint);
        if (lib->load())
        {
            bm->m_behaviourHash_To_library[hash] = lib;

            // Notify the BehaviourHolder
            List<BehaviourHolder*> behDemanders =
                    bm->m_behHash_To_behHolderDemanders[hash];

            for (BehaviourHolder* bh : behDemanders)
            {
                bh->OnBehaviourLibraryAvailable(lib);
            }

            bm->m_behHash_To_behHolderDemanders.Remove(hash);

            // Remove the outdated libraries files
            RemoveOutdatedLibraryFiles(libraryFilepath);
        }
        else
        {
            Debug_Error("There was an error when loading the library '" <<
                        libraryFilepath << "': " << lib->errorString().toStdString());
        }
    }
    m_behFilepath_To_compiledLibrary.Clear();
}

void BehaviourManager::RemoveOutdatedLibraryFiles(const String &mostRecentLibraryFilepath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance(); ASSERT(bm);

    String libraryNameAndExtension =
            Persistence::GetFileNameWithExtension(mostRecentLibraryFilepath);
    String libraryName = Persistence::GetFileName(mostRecentLibraryFilepath);

    List<String> libs = Persistence::GetFiles(Persistence::GetProjectAssetsRootAbs(),
                                              true, {"*.so.*"});
    for (auto it = libs.Begin(); it != libs.End(); ++it)
    {
        const String &libFilepath = *it;
        bool isOfTheSameBehaviour = (Persistence::GetFileName(libFilepath) == libraryName);
        bool isTheMostRecentOne = libFilepath.EndsWith(libraryNameAndExtension);
        if (isOfTheSameBehaviour && !isTheMostRecentOne)
        {
            Persistence::Remove(libFilepath); // Remove outdated library
        }
    }
}

bool BehaviourManager::IsCached(const String &behaviourPath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    ASSERT(bm, "", return false);

    String absPath = Persistence::ToAbsolute(behaviourPath, false);
    String hash = Persistence::GetHash(absPath);
    return bm->m_behaviourHash_To_library.ContainsKey(hash);
}

QLibrary *BehaviourManager::GetCachedLibrary(const String &behaviourPath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    ASSERT(bm, "", return nullptr);

    String absPath = Persistence::ToAbsolute(behaviourPath, false);
    String hash = Persistence::GetHash(absPath);
    return bm->m_behaviourHash_To_library[hash];
}

bool BehaviourManager::IsBeingCompiled(const String &behaviourPath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    ASSERT(bm, "", return false);

    String absPath = Persistence::ToAbsolute(behaviourPath, false);

    return bm->m_behPathsBeingCompiled.find(absPath) !=
           bm->m_behPathsBeingCompiled.end();
}

void BehaviourManager::Load(BehaviourHolder *behaviourHolder,
                            const String &behaviourFilepath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    ASSERT(bm);

    String absPath = Persistence::ToAbsolute(behaviourFilepath, false);
    String hash = Persistence::GetHash(absPath);
    if (bm->m_failed_behFilepath_To_behaviourHash.ContainsKey(behaviourFilepath))
    {
        String oldHash = bm->m_failed_behFilepath_To_behaviourHash[behaviourFilepath];
        if (oldHash == hash)
        {
            // A previous compilation of behaviourFilepath has failed,
            // and the file hasn't changed (same hash)
            return;
        }
        else
        {
            bm->m_failed_behFilepath_To_behaviourHash.Remove(behaviourFilepath);
        }
    }

    if (BehaviourManager::IsCached(absPath))
    {
        // It's cached from a previous load...
        QLibrary *lib = BehaviourManager::GetCachedLibrary(absPath);
        behaviourHolder->OnBehaviourLibraryAvailable(lib);
    }
    else
    {
        // Add behaviour to the list of demanders
        if (!bm->m_behHash_To_behHolderDemanders.ContainsKey(hash))
        {   // Init list
            bm->m_behHash_To_behHolderDemanders[hash] = List<BehaviourHolder*>();
        }

        if (!bm->m_behHash_To_behHolderDemanders[hash].Contains(behaviourHolder))
        {
            bm->m_behHash_To_behHolderDemanders[hash].PushBack(behaviourHolder);
        }

        if (!BehaviourManager::IsBeingCompiled(absPath))
        {
            // Compile once
            bm->m_behPathsBeingCompiled.insert(absPath);

            #ifdef BANG_EDITOR
            // Have to compile and load it. First compile
            BehaviourManagerCompileThread *compileThread =
                    new BehaviourManagerCompileThread(absPath);
            compileThread->start();

            Debug_Status("Compiling script " <<
                         Persistence::GetFileName(behaviourFilepath) << "...", 5.0f);
            // And when the compileThread finishes, we will be notified,
            // load the library, and then notify the behaviourHolders waiting for it

            #else // GAME
            // In Game, we have the behaviour object library in
            // "BEHAVIOUR_DIR/BEHAVIOUR_NAME.so.RANDOM_PROJECT_ID"
            // so we just have to load it directly. I.E., notify the instant load.
            const String behaviourDir = Persistence::GetDir(behaviourFilepath);
            const String behaviourFilename = Persistence::GetFileName(behaviourFilepath);
            const String libraryFilepath = behaviourDir + "/" + behaviourFilename + ".so." +
                    ProjectManager::GetCurrentProject()->GetProjectRandomId();
            //Debug_Log("Loading library " << libraryFilepath);
            bm->OnBehaviourFinishedCompiling(behaviourFilepath, libraryFilepath);
            #endif
        }
    }
}

void BehaviourManager::
    OnBehaviourHolderDeleted(BehaviourHolder *behaviourHolder)
{
    BehaviourManager *bm = BehaviourManager::GetInstance(); ASSERT(bm);

    // Erase the behaviourHolder from all the demand lists it is in
    for (auto it = bm->m_behHash_To_behHolderDemanders.Begin();
         it != bm->m_behHash_To_behHolderDemanders.End(); ++it)
    {
        List<BehaviourHolder*> &bhList = it->second;
        bhList.RemoveAll(behaviourHolder);
    }
}
