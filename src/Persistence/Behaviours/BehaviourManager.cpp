#include "BehaviourManager.h"

#include <QLibrary>

#include "Debug.h"
#include "Scene.h"
#include "Project.h"
#include "Persistence.h"
#include "Application.h"
#include "SystemUtils.h"
#include "SceneManager.h"
#include "ProjectManager.h"
#include "BehaviourHolder.h"
#include "BehaviourCompileRunnable.h"

BehaviourManager::BehaviourManager()
{
    // Only compile N behaviours at a time
    m_threadPool.setMaxThreadCount(3);
}

QLibrary *BehaviourManager::LoadLibraryFromFilepath(const String &libFilepath)
{
    QLibrary *lib = new QLibrary(libFilepath.ToCString());
    lib->setLoadHints(QLibrary::LoadHint::ResolveAllSymbolsHint);
    if (lib->load())
    {
        return lib;
    }

    Debug_Error("There was an error when loading the library '" <<
                 libFilepath << "': " << lib->errorString());
    return nullptr;
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

    String hash = BehaviourManager::GetHash(behaviourPath);
    m_behHashesBeingCompiled.erase(hash);

    QLibrary *library = LoadLibraryFromFilepath(libraryFilepath);
    if (library)
    {
        // Notify the BehaviourHolder's that were waiting this library
        const List<BehaviourHolder*> &behDemanders =
                m_behHash_To_demandersList[hash];

        for (BehaviourHolder* bh : behDemanders)
        {
            bh->OnBehaviourLibraryAvailable(library);
        }

        m_behHash_To_demandersList.Remove(hash);
        m_behHash_To_lib.Set(hash, library);

        RemoveOutdatedLibraryFiles(libraryFilepath);
    }
}

void BehaviourManager::OnBehaviourFailedCompiling(const String &behaviourPath)
{
    QMutexLocker locker(&m_mutex);
    String hash = BehaviourManager::GetHash(behaviourPath);
    m_behHashesBeingCompiled.erase(hash);
    m_failedBehHashes.insert(hash);
}

void BehaviourManager::RemoveOutdatedLibraryFiles(
        const String &mostRecentLibFilepath)
{
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

bool BehaviourManager::IsCached(const String &hash)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    return  bm->m_behHash_To_lib.ContainsKey(hash) &&
           !BehaviourManager::IsBeingCompiled(hash);
}

bool BehaviourManager::AllBehaviourHoldersUpdated(float *percentOfBehavioursUpdated)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    QMutexLocker locker(&bm->m_mutex);

    int behHoldersUpdated = 0;
    Scene *scene = SceneManager::GetActiveScene();
    List<BehaviourHolder*> behHolders = scene->GetComponentsInChildren<BehaviourHolder>();
    for (BehaviourHolder *bh : behHolders)
    {
        String srcPath = bh->GetSourceFilepath();
        String hash = GetHash(srcPath);

        QLibrary *updatedLibrary = BehaviourManager::GetCachedLibrary(hash);
        if (!updatedLibrary) { continue; } // Not even compiled

        QLibrary *currentBHLibrary = bh->GetLibraryBeingUsed();

        bool updated = (currentBHLibrary == updatedLibrary);
        if (updated) { ++behHoldersUpdated; }
    }

    float percentUpdated = float(behHoldersUpdated) / behHolders.Size();
    if (percentOfBehavioursUpdated)
    {
        *percentOfBehavioursUpdated = percentUpdated;
    }

    return percentUpdated == 1.0f;
}

bool BehaviourManager::SomeBehaviourWithError()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    QMutexLocker locker(&bm->m_mutex);

    Scene *scene = SceneManager::GetActiveScene();
    List<BehaviourHolder*> behHolders = scene->GetComponentsInChildren<BehaviourHolder>();
    for (BehaviourHolder *bh : behHolders)
    {
        String srcPath = bh->GetSourceFilepath();
        String hash = GetHash(srcPath);
        if (bm->m_failedBehHashes.count(hash) > 0)
        {
            return true;
        }
    }
    return false;
}

String BehaviourManager::GetHash(const String &behaviourPath)
{
    String absPath = Persistence::ToAbsolute(behaviourPath, false);
    return Persistence::GetHash(absPath);
}

QLibrary *BehaviourManager::GetCachedLibrary(const String &hash)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    if (bm->m_behHash_To_lib.ContainsKey(hash))
    {
        return bm->m_behHash_To_lib[hash];
    }
    return nullptr;
}

bool BehaviourManager::IsBeingCompiled(const String &hash)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();

    // Is being demanded but it has no compiled library associated,
    // this means the behaviour is still being compiled :)
    return  bm->m_behHashesBeingCompiled.count(hash) > 0;
}

void BehaviourManager::Load(BehaviourHolder *behaviourHolder,
                            const String &behaviourFilepath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    QMutexLocker locker(&bm->m_mutex);

    String hash = BehaviourManager::GetHash(behaviourFilepath);
    if (bm->m_failedBehHashes.count(hash) > 0)
    {
        return; // This hash has failed before, dont try to recompile it
    }

    if (BehaviourManager::IsCached(hash))
    {
        // It's cached from a previous load...
        QLibrary *lib = BehaviourManager::GetCachedLibrary(hash);
        if (lib)
        {
            behaviourHolder->OnBehaviourLibraryAvailable(lib);
        }
    }
    else
    {
        // Add behaviour to the list of demanders
        List<BehaviourHolder*> &demanders = bm->m_behHash_To_demandersList.Get(hash);
        if (!demanders.Contains(behaviourHolder))
        {
            demanders.PushBack(behaviourHolder);
        }

        if ( !BehaviourManager::IsBeingCompiled(hash) )
        {
            #ifdef BANG_EDITOR

            // Have to compile and load it.
            // First compile
            BehaviourCompileRunnable *compileRunnable =
                    new BehaviourCompileRunnable(behaviourFilepath);
            bool startedCompiling = bm->m_threadPool.tryStart(compileRunnable);
            if (!startedCompiling)
            {
                return;
            }

            bm->m_behHashesBeingCompiled.insert(hash);

            String behaviourName = Persistence::GetFileName(behaviourFilepath);
            Debug_Status("Compiling script " << behaviourName << "...", 5.0f);

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
            locker.unlock();
            bm->OnBehaviourFinishedCompiling(behaviourFilepath, libFilepath);
            #endif
        }
    }
}

void BehaviourManager::
    OnBehaviourHolderDeleted(BehaviourHolder *behaviourHolder)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    QMutexLocker locker(&bm->m_mutex);

    // Erase the behaviourHolder from all the demand lists it is in
    for (auto &hash_demandersList : bm->m_behHash_To_demandersList)
    {
        List<BehaviourHolder*> &bhList = hash_demandersList.second;
        bhList.RemoveAll(behaviourHolder);
    }
}

void BehaviourManager::RefreshAllBehaviours()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    bm->m_behaviourRefresherTimer.RefreshBehavioursInScene();
}
