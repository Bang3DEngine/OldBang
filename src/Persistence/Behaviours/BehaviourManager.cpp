#include "BehaviourManager.h"

#include <QLibrary>

#include "Debug.h"
#include "Scene.h"
#include "Project.h"
#include "ListLogger.h"
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
void BehaviourManager::OnBehaviourSuccessCompiling(const QString &_behaviourPath,
                                                   const QString &_libraryFilepath,
                                                   const QString &_warnMessage)
{
    String behaviourPath   = _behaviourPath;
    String libraryFilepath = _libraryFilepath;
    String warnMessage     = _warnMessage;
    QLibrary *library = LoadLibraryFromFilepath(libraryFilepath);
    m_status.OnBehaviourSuccessCompiling(behaviourPath,
                                          libraryFilepath,
                                          warnMessage,
                                          library);

    if (!warnMessage.Empty())
    {
        Debug_Warn(warnMessage);
    }

    if (library)
    {
        BehaviourId bid(behaviourPath);

        // Notify the BehaviourHolder's that were waiting this library
        const List<BehaviourHolder*> &behDemanders = m_status.GetDemanders(bid);
        for (BehaviourHolder* bh : behDemanders)
        {
            bh->OnBehaviourLibraryAvailable(library);
        }
        RemoveOutdatedLibraryFiles(libraryFilepath);
    }
}

void BehaviourManager::OnBehaviourFailedCompiling(const QString &behaviourPath,
                                                  const QString &errorMessage)
{
    m_status.OnBehaviourFailedCompiling(String(behaviourPath),
                                        String(errorMessage));
}

void BehaviourManager::RemoveOutdatedLibraryFiles(const String &newLibFilepath)
{
    String mostRecentLibNameAndExt =
            Persistence::GetFileNameWithExtension(newLibFilepath);
    String mostRecentLibName = Persistence::GetFileName(newLibFilepath);
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

void BehaviourManager::Load(BehaviourHolder *bHolder, const String &behaviourPath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    BehaviourId bid(behaviourPath);

    // See if the behaviour has changed from the cached one, and
    // in that case remove the outdated references.
    bm->m_status.TreatIfBehaviourChanged(behaviourPath);

    if (bm->m_status.HasFailed(bid)) { return; }
    if (bm->m_status.IsBeingCompiled(bid)) { return; }

    if (!bm->m_status.IsCached(bid))
    {
        // Add to demander list
        bm->m_status.OnBehaviourDemanded(behaviourPath, bHolder);
    }

    if (bm->m_status.IsCached(bid))
    {
        QLibrary *lib = bm->m_status.GetLibrary(bid);
        if (lib)
        {
            bHolder->OnBehaviourLibraryAvailable(lib);
        }
    }
    else // Add behaviour to the list of demanders
    {
        if (!bm->m_status.IsBeingCompiled(bid))
        {
            #ifdef BANG_EDITOR
            // Have to compile and load it.
            // First compile
            BehaviourCompileRunnable *compileRunnable =
                    new BehaviourCompileRunnable(behaviourPath);
            bool startedCompiling = bm->m_threadPool.tryStart(compileRunnable);
            if (!startedCompiling)
            {
                return;
            }

            bm->m_status.OnBehaviourStartedCompiling(behaviourPath);

            String behaviourName = Persistence::GetFileName(behaviourPath);
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

void BehaviourManager::OnBehaviourHolderDeleted(BehaviourHolder *behaviourHolder)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    bm->m_status.OnBehaviourHolderDeleted(behaviourHolder);
}

void BehaviourManager::RefreshAllBehaviours()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    bm->m_behaviourRefresherTimer.RefreshBehavioursInScene();
}

const BehaviourManagerStatus &BehaviourManager::GetStatus()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    return bm->m_status;
}
