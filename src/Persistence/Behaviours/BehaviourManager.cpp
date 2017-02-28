#include "BehaviourManager.h"

#include <QLibrary>

#include "Time.h"
#include "Debug.h"
#include "Scene.h"
#include "Project.h"
#include "StringUtils.h"
#include "Persistence.h"
#include "Application.h"
#include "SystemUtils.h"
#include "SceneManager.h"
#include "ProjectManager.h"
#include "BehaviourHolder.h"
#include "CodePreprocessor.h"
#include "BehaviourMergeObjectsRunnable.h"
#include "BehaviourObjectCompileRunnable.h"

#ifdef BANG_EDITOR
#include "Console.h"
#endif

BehaviourManager::BehaviourManager()
{
    m_threadPool.setMaxThreadCount(5);
}

BehaviourManager *BehaviourManager::GetInstance()
{
    Application *app = Application::GetInstance();
    return app ? app->m_behaviourManager : nullptr;
}

void BehaviourManager::RemoveMergedLibraryFiles()
{
    List<String> libFilepaths = Persistence::GetFiles(
                BehaviourManager::GetCurrentLibsDir(), true, {"*.so.*"});
    for (const String &libFilepath : libFilepaths)
    {
        Persistence::Remove(libFilepath);
    }
}

QLibrary *BehaviourManager::GetBehavioursMergedLibrary()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    return bm->m_behavioursLibrary;
}

List<String> BehaviourManager::GetBehavioursSourcesFilepathsList()
{
    return Persistence::GetFiles(Persistence::GetProjectAssetsRootAbs(),
                                 true, {"cpp"});
}

List<String> BehaviourManager::GetBehavioursObjectsFilepathsList()
{
    return Persistence::GetFiles(BehaviourManager::GetCurrentLibsDir(),
                                 true, {"o"});
}

bool BehaviourManager::PrepareBehavioursLibrary(bool forGame, bool *stopFlag)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    #ifdef BANG_EDITOR
    do
    {
        bm->StartCompilingAllBehaviourObjects(forGame);

        float percent = bm->m_status.GetPercentOfReadyBehaviours();
        emit bm->NotifyPrepareBehavioursLibraryProgressed( int(percent * 100) );
        if (stopFlag && *stopFlag) { return false; }

        QThread::currentThread()->msleep(100);
        Application::GetInstance()->processEvents();
    }
    while(!bm->m_status.AllBehavioursReadyOrFailed());

    emit bm->NotifyPrepareBehavioursLibraryProgressed(99);

    // Now merge them
    bool error = !bm->m_status.AllBehavioursReady();
    if (!error && !bm->m_status.IsBehavioursLibraryReady())
    {
        bool mergingStarted = false;
        do
        {
            mergingStarted =
                    BehaviourManager::StartMergingBehavioursObjects(forGame);
            QThread::currentThread()->msleep(100);
            Application::processEvents();
        }
        while (!mergingStarted || BehaviourManager::GetMergeState() ==
               BehaviourManager::MergingState::Merging);

        error = (BehaviourManager::GetMergeState() !=
                BehaviourManager::MergingState::Success);
        if (!error) { bm->m_status.OnBehavioursLibraryReady(); }
    }
    return !error;
    #else
    // In game, the library is compiled from before.
    Project *project = ProjectManager::GetCurrentProject();
    String libDir = Persistence::GetProjectLibsRootAbs();
    String projId = project->GetProjectRandomId();
    String libFilepath = "Behaviours.so." + projId + ".1.1";
    String libOutput = libDir + "/" + libFilepath;
    bm->OnMergedLibraryCompiled(libOutput.ToQString(), "");
    return true;
    #endif
}



void BehaviourManager::SetCurrentLibsDir(const String &libsDir)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    bm->m_currentLibsDir = libsDir;
}

const String &BehaviourManager::GetCurrentLibsDir()
{
    return BehaviourManager::GetInstance()->m_currentLibsDir;
}

bool BehaviourManager::StartMergingBehavioursObjects(bool forGame)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    if (!bm->GetStatus().AllBehavioursReady()) { return false; }
    if (BehaviourManager::GetMergeState() == MergingState::Merging)
    {
        return true;
    }

    RemoveMergedLibraryFiles();
    BehaviourMergeObjectsRunnable *mergeRunn =
            new BehaviourMergeObjectsRunnable(forGame);
    bool mergingStarted = bm->m_threadPool.tryStart(mergeRunn);
    if (mergingStarted)
    {
        connect(mergeRunn, SIGNAL(NotifySuccessMerging(QString, QString)),
                bm, SLOT(OnMergedLibraryCompiled(QString, QString)));
        connect(mergeRunn, SIGNAL(NotifyFailedMerging(QString)),
                bm, SLOT(OnMergedLibraryCompilationFailed(QString)));
        bm->m_state = MergingState::Merging;
    }
    else { delete mergeRunn; }

    return mergingStarted;
}

void BehaviourManager::StartCompilingAllBehaviourObjects(bool forGame)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    List<String> allBehaviourSources =
            BehaviourManager::GetBehavioursSourcesFilepathsList();
    for (const String &behFilepath : allBehaviourSources)
    {
        if (!bm->m_status.HasFailed(behFilepath) &&
                !bm->m_status.IsReady(behFilepath) &&
                !bm->m_status.IsBeingCompiled(behFilepath))
        {
            BehaviourManager::StartCompilingBehaviourObject(behFilepath,
                                                            forGame);
        }
    }
}

void BehaviourManager::StartCompilingBehaviourObject(const String &behFilepath,
                                                     bool forGame)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();

    if (bm->GetStatus().IsReady(behFilepath) ||
        bm->GetStatus().IsBeingCompiled(behFilepath) ||
        bm->GetStatus().HasFailed(behFilepath))
    {
        return;
    }

    Debug_Status("Compiling " << behFilepath, 3.0f);

    BehaviourObjectCompileRunnable *objRunn =
            new BehaviourObjectCompileRunnable(behFilepath, forGame);
    bool started = bm->m_threadPool.tryStart(objRunn);
    if (started)
    {
        connect(objRunn, SIGNAL(NotifySuccessCompiling(QString,QString)),
                bm, SLOT(OnBehaviourObjectCompiled(QString,QString)));
        connect(objRunn, SIGNAL(NotifyFailedCompiling(QString, QString)),
                bm, SLOT(OnBehaviourObjectCompilationFailed(QString, QString)));
        bm->m_status.OnBehaviourStartedCompiling(behFilepath);
    }
    else { delete objRunn; }
}

BehaviourManager::MergingState BehaviourManager::GetMergeState()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    return bm->m_state;
}

const BehaviourManagerStatus &BehaviourManager::GetStatus()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    return bm->m_status;
}

void BehaviourManager::OnBehaviourObjectCompiled(const QString &behFilepath,
                                                 const QString &warnMessage)
{
    m_status.OnBehaviourSuccessCompiling(behFilepath);
    String warn(warnMessage);
    if (!warn.Empty()) { Debug_Warn(warn); }
}

void BehaviourManager::OnBehaviourObjectCompilationFailed(
        const QString &behaviourFilepath,
        const QString &errorMessage)
{
    m_status.OnBehaviourFailedCompiling(behaviourFilepath, errorMessage);
}

void BehaviourManager::OnMergedLibraryCompiled(QString libFilepath,
                                               QString warnMessage)
{
    String warn(warnMessage);
    if (!warn.Empty()) { Debug_Warn(warn); }

    QLibrary *behavioursLib = new QLibrary(libFilepath);
    behavioursLib->setLoadHints(QLibrary::LoadHint::ResolveAllSymbolsHint);
    bool success = behavioursLib->load();

    m_behavioursLibrary = success ? behavioursLib : nullptr;
    m_state = success ? MergingState::Success : MergingState::Failed;

    if (!success)
    {
        Debug_Error("There was an error when loading the library '" <<
                     libFilepath << "': " << behavioursLib->errorString());
    }
}

void BehaviourManager::OnMergedLibraryCompilationFailed(QString errorMessage)
{
    Debug_Error("Error while merging behaviour objects into a single shared "
                << "library: " << errorMessage);
    m_state = MergingState::Failed;
}
