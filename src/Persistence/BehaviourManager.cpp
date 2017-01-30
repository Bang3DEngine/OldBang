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
    m_behPath_To_compiledLibraryPath.Set(absPath, libraryFilepath);
}

void BehaviourManager::OnBehaviourFailedCompiling(const String &behaviourPath)
{
    QMutexLocker locker(&m_mutex);
    String absPath = Persistence::ToAbsolute(behaviourPath, false);
    m_behPathsBeingCompiled.erase(absPath);
    m_failed_behPath_To_behHash.Set(behaviourPath,
                                              Persistence::GetHash(absPath));
}

void BehaviourManager::TreatCompiledBehaviours()
{
    QMutexLocker locker(&m_mutex);

    for (auto it = m_behPath_To_compiledLibraryPath.Begin();
         it != m_behPath_To_compiledLibraryPath.End(); ++it)
    {
        String behaviourPath = it->first;
        String absPath = Persistence::ToAbsolute(behaviourPath, false);
        String hash = Persistence::GetHash(absPath);

        BehaviourManager *bm = BehaviourManager::GetInstance();

        // Open the library
        String libraryFilepath = it->second;
        QLibrary *lib = new QLibrary(libraryFilepath.ToCString());
        lib->setLoadHints(QLibrary::LoadHint::ResolveAllSymbolsHint);
        if (lib->load())
        {
            bm->m_behHash_To_lib[hash] = lib;

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
                        libraryFilepath << "': " << lib->errorString());
        }
    }
    m_behPath_To_compiledLibraryPath.Clear();
}

void BehaviourManager::RemoveOutdatedLibraryFiles(
        const String &mostRecentLibFilepath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance(); ASSERT(bm);

    String mostRecentLibNameAndExt =
            Persistence::GetFileNameWithExtension(mostRecentLibFilepath);
    String mostRecentLibName = Persistence::GetFileName(mostRecentLibFilepath);
    List<String> libs = Persistence::GetFiles(
                Persistence::GetProjectAssetsRootAbs(), true, {"*.so.*"});

    for (auto it = libs.Begin(); it != libs.End(); ++it)
    {
        const String &libFilepath = *it;
        String libFileName = Persistence::GetFileName(libFilepath);
        bool isOfTheSameBehaviour = (libFileName == mostRecentLibName);
        bool isTheMostRecentOne = libFilepath.EndsWith(mostRecentLibNameAndExt);
        if (isOfTheSameBehaviour && !isTheMostRecentOne)
        {
            Persistence::Remove(libFilepath);
        }
    }
}

bool BehaviourManager::IsCached(const String &behaviourPath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    ASSERT(bm, "", return false);

    String absPath = Persistence::ToAbsolute(behaviourPath, false);
    String hash = Persistence::GetHash(absPath);
    return bm->m_behHash_To_lib.ContainsKey(hash);
}

QLibrary *BehaviourManager::GetCachedLibrary(const String &behaviourPath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    ASSERT(bm, "", return nullptr);

    String absPath = Persistence::ToAbsolute(behaviourPath, false);
    String hash = Persistence::GetHash(absPath);
    return bm->m_behHash_To_lib[hash];
}

void BehaviourManager::RefreshAllBehaviourHoldersSynchronously()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    bm->m_behaviourRefresherTimer.RefreshBehavioursInScene(true);
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
                            const String &behaviourFilepath,
                            bool synchronously)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    ASSERT(bm);

    String behAbsPath = Persistence::ToAbsolute(behaviourFilepath, false);
    String hash = Persistence::GetHash(behAbsPath);
    if (bm->m_failed_behPath_To_behHash.ContainsKey(behaviourFilepath))
    {
        String oldHash = bm->m_failed_behPath_To_behHash[behaviourFilepath];
        if (oldHash != hash)
        {
            // If a previous compilation of behaviourFilepath has failed,
            // and the file has changed (different hash)

            bm->m_failed_behPath_To_behHash.Remove(behaviourFilepath);
        }
    }

    if (BehaviourManager::IsCached(behAbsPath))
    {
        // It's cached from a previous load...
        QLibrary *lib = BehaviourManager::GetCachedLibrary(behAbsPath);
        behaviourHolder->OnBehaviourLibraryAvailable(lib);
    }
    else
    {
        // Add behaviour to the list of demanders
        if (!bm->m_behHash_To_behHolderDemanders.ContainsKey(hash))
        {
            // Init list
            bm->m_behHash_To_behHolderDemanders[hash] = List<BehaviourHolder*>();
        }

        if (!bm->m_behHash_To_behHolderDemanders[hash].Contains(behaviourHolder))
        {
            bm->m_behHash_To_behHolderDemanders[hash].PushBack(behaviourHolder);
        }

        if (!synchronously)
        {
            if (!BehaviourManager::IsBeingCompiled(behAbsPath))
            {
                // Compile once
                bm->m_behPathsBeingCompiled.insert(behAbsPath);

                #ifdef BANG_EDITOR

                // Have to compile and load it.
                // First compile
                BehaviourManagerCompileThread *compileThread =
                        new BehaviourManagerCompileThread(behAbsPath);
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
                const String projRandomId = ProjectManager::GetCurrentProject()->GetProjectRandomId();
                const String libFilepath = behaviourDir + "/" + behaviourFilename + ".so." +
                                           projRandomId;
                bm->OnBehaviourFinishedCompiling(behaviourFilepath, libFilepath);
                bm->TreatCompiledBehaviours();
                #endif
            }
        }
        else
        {
            if (!BehaviourManager::IsCached(behAbsPath))
            {
                // Its a thread, but called synchronously
                BehaviourManagerCompileThread *compileThread =
                        new BehaviourManagerCompileThread(behAbsPath);
                compileThread->Compile();
            }
            else
            {
                String libFilepath = bm->m_behPath_To_compiledLibraryPath[hash];
                bm->OnBehaviourFinishedCompiling(behaviourFilepath,
                                                 libFilepath);
            }
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
