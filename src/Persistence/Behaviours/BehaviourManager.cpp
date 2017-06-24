#include "Bang/BehaviourManager.h"

#include <QLibrary>
#include "Bang/WinUndef.h"

#include "Bang/Paths.h"
#include "Bang/Time.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Project.h"
#include "Bang/Behaviour.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/ProjectManager.h"
#include "Bang/CodePreprocessor.h"
#include "Bang/BehaviourMergeObjectsRunnable.h"
#include "Bang/BehaviourObjectCompileRunnable.h"

#ifdef BANG_EDITOR
#include "Bang/Console.h"
#endif

BehaviourManager::BehaviourManager()
{
    m_threadPool.setMaxThreadCount(2);
}

BehaviourManager *BehaviourManager::GetInstance()
{
    Application *app = Application::GetInstance();
    return app ? app->m_behaviourManager : nullptr;
}

void BehaviourManager::RemoveMergedLibraryFiles()
{
    List<Path> libFilepaths =
           BehaviourManager::GetCurrentLibsDir().GetFiles(true, {"*.so.*"});
    for (const Path &libFilepath : libFilepaths)
    {
        File::Remove(libFilepath);
    }
}

QLibrary *BehaviourManager::GetBehavioursMergedLibrary()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    return bm->m_behavioursLibrary;
}

List<Path> BehaviourManager::GetBehavioursSourcesFilepathsList()
{
    return Paths::ProjectAssets().GetFiles(true, {"cpp"});
}

List<Path> BehaviourManager::GetBehavioursObjectsFilepathsList()
{
    return Path(BehaviourManager::GetCurrentLibsDir()).GetFiles(true, {"o"});
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

    // Invalidating will force a new library with new name reload, and thus
    // forcing static reinitialization, instead of them keeping latest lib
    // values, which is exactly what we want (static reinitialization)
    bm->m_status.InvalidateBehavioursLibraryReady();
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
    if (!BehaviourManager::GetBehavioursMergedLibrary())
    {
        Project *project = ProjectManager::GetCurrentProject();
        Path libDir = Paths::ProjectLibraries();
        String projId = project->GetProjectRandomId();
        String libFilepath = "Behaviours.so." + projId + ".1.1";
        Path libOutput = libDir.Append(libFilepath);
        bm->OnMergedLibraryCompiled(libOutput.GetAbsolute().ToQString(), true, "");
    }
    return true;
    #endif
}



void BehaviourManager::SetCurrentLibsDir(const Path &libsDir)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    bm->m_currentLibsDir = libsDir;
}

const Path &BehaviourManager::GetCurrentLibsDir()
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
        connect(mergeRunn, SIGNAL(NotifySuccessMerging(QString, bool, QString)),
                bm, SLOT(OnMergedLibraryCompiled(QString, bool, QString)));
        connect(mergeRunn, SIGNAL(NotifyFailedMerging(QString)),
                bm, SLOT(OnMergedLibraryCompilationFailed(QString)));
        bm->m_state = MergingState::Merging;
    }
    else { delete mergeRunn; }

    return mergingStarted;
}

void BehaviourManager::StartCompilingAllBehaviourObjects(bool forGame)
{
    List<Path> allBehaviourSources =
            BehaviourManager::GetBehavioursSourcesFilepathsList();
    for (const Path &behFilepath : allBehaviourSources)
    {
        BehaviourManager::StartCompilingBehaviourObject(behFilepath, forGame);
    }
}

void BehaviourManager::StartCompilingBehaviourObject(const Path &behFilepath,
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
        connect(objRunn, SIGNAL(NotifySuccessCompiling(QString,bool,QString)),
                bm, SLOT(OnBehaviourObjectCompiled(QString,bool,QString)));
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
                                                 bool mergingForGame,
                                                 const QString &warnMessage)
{
    m_status.OnBehaviourSuccessCompiling( Path( String(behFilepath) ) );
    String warn(warnMessage);
    if (!warn.Empty()) { Debug_Warn(warn); }

    BehaviourManager *bm = BehaviourManager::GetInstance();
    if (bm->m_status.AllBehavioursReady())
    {
        BehaviourManager::StartMergingBehavioursObjects(mergingForGame);
    }
}

void BehaviourManager::OnBehaviourObjectCompilationFailed(
        const QString &behaviourFilepath,
        const QString &errorMessage)
{
    m_status.OnBehaviourFailedCompiling( Path( String(behaviourFilepath) ),
                                         errorMessage);
}

void BehaviourManager::OnMergedLibraryCompiled(QString libFilepath,
                                               bool loadLibrary,
                                               QString warnMessage)
{
    String warn(warnMessage);
    if (!warn.Empty()) { Debug_Warn(warn); }

    bool success = true;
    if (loadLibrary)
    {
        QLibrary *behavioursLib = new QLibrary(libFilepath);
        behavioursLib->setLoadHints(QLibrary::LoadHint::ResolveAllSymbolsHint);
        success = behavioursLib->load();
        m_behavioursLibrary = success ? behavioursLib : nullptr;

        if (!success)
        {
            Debug_Error("There was an error when loading the library '" <<
                         libFilepath << "': " << behavioursLib->errorString());
        }
    }

    m_state = success ? MergingState::Success : MergingState::Failed;
}

void BehaviourManager::OnMergedLibraryCompilationFailed(QString errorMessage)
{
    Debug_Error("Error while merging behaviour objects into a single shared "
                << "library: " << errorMessage);
    m_state = MergingState::Failed;
}
