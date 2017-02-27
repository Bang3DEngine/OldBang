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
#include "BehaviourObjectCompileRunnable.h"
#include "CodePreprocessor.h"

#ifdef BANG_EDITOR
#include "Console.h"
#endif

BehaviourManager::BehaviourManager()
{
    m_behaviourObjectCompileThreadPool.setMaxThreadCount(5);
}

BehaviourManager *BehaviourManager::GetInstance()
{
    Application *app = Application::GetInstance();
    return app ? app->m_behaviourManager : nullptr;
}

void BehaviourManager::RemoveMergedLibraryFiles()
{
    const String libsDir = Persistence::GetProjectLibsRootAbs();
    List<String> libFilepaths = Persistence::GetFiles(libsDir, true, {"*.so.*"});
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
    return Persistence::GetFiles(Persistence::GetProjectLibsRootAbs(),
                                 true, {"o"});
}

void BehaviourManager::StartMergingBehavioursObjects()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();

    Debug_Log("StartMergingBehavioursObjects");
    if (BehaviourManager::GetState() ==
            BehaviourManager::MergingState::Merging)
    {
        Debug_Log("  Cancelled, currently merging...");
        return;
    }

    RemoveMergedLibraryFiles();

    List<String> allBehaviourSources =
            BehaviourManager::GetBehavioursSourcesFilepathsList();
    Debug_Log(allBehaviourSources);

    Debug_Log("  Waiting for all behaviour objects to be ready!");
    bool allBehaviourObjectsReady = false;
    while (!allBehaviourObjectsReady)
    {
        allBehaviourObjectsReady = true;
        for (const String &behFilepath : allBehaviourSources)
        {
            Debug_Log ("  Is " << behFilepath << " ready? "
                       << bm->m_status.IsReady(behFilepath));

            if (bm->m_status.HasFailed(behFilepath))
            {
                bm->m_state = BehaviourManager::MergingState::Failed;
                return;
            }

            if (!bm->m_status.IsReady(behFilepath))
            {
                allBehaviourObjectsReady = false;
                if (!bm->m_status.IsBeingCompiled(behFilepath))
                {
                    StartCompilingBehaviourObject(behFilepath);
                }
            }
        }
        Application::GetInstance()->processEvents();
        QThread::currentThread()->msleep(500);
        Application::GetInstance()->processEvents();
    }

    Debug_Log("  All Behaviour Objects Ready!");

    bm->m_state = MergingState::Merging;

    Debug_Log("  Starting merging...");
    bm->moveToThread(&bm->m_mergeObjectsThread);
    QObject::connect(&bm->m_mergeObjectsThread, SIGNAL(started()),
                     bm, SLOT(CompileMergedLibrary()));
    bm->m_mergeObjectsThread.start();
}

void BehaviourManager::StartCompilingBehaviourObject(
        const String &behaviourFilepath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();

    Debug_Status("Compiling " << behaviourFilepath, 3.0f);

    BehaviourObjectCompileRunnable *objRunn =
            new BehaviourObjectCompileRunnable(behaviourFilepath);
    bool started = bm->m_behaviourObjectCompileThreadPool.tryStart(objRunn);
    if (started)
    {
        connect(objRunn, SIGNAL(NotifySuccessCompiling(QString,QString)),
                bm, SLOT(OnBehaviourObjectCompiled(QString,QString)),
                Qt::DirectConnection);
        connect(objRunn, SIGNAL(NotifyFailedCompiling(QString, QString)),
                bm, SLOT(OnBehaviourObjectCompilationFailed(QString, QString)),
                Qt::DirectConnection);
        bm->m_status.OnBehaviourStartedCompiling(behaviourFilepath);
    }
}

BehaviourManager::MergingState BehaviourManager::GetState()
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
    Debug_Log("OnBehaviourObjectCompiled " << behFilepath);
    m_status.OnBehaviourSuccessCompiling(behFilepath);
    String warn(warnMessage);
    if (!warn.Empty()) { Debug_Warn(warn); }
}

void BehaviourManager::OnBehaviourObjectCompilationFailed(
        const QString &behaviourFilepath,
        const QString &errorMessage)
{
    Debug_Log("OnBehaviourObjectCompilationFailed " << behaviourFilepath);
    m_status.OnBehaviourFailedCompiling(behaviourFilepath);
    Debug_Error("There was an error compiling " << behaviourFilepath << ": " <<
                errorMessage);
}

void BehaviourManager::OnMergedLibraryCompiled(const QString &libFilepath,
                                               const QString &warnMessage)
{
    String warn(warnMessage);
    if (!warn.Empty()) { Debug_Warn(warn); }

    Debug_Log("OnBehavioursLibraryCompiled " << libFilepath);

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

void BehaviourManager::OnMergedLibraryCompilationFailed(
        const QString &errorMessage)
{
    Debug_Error("Error while merging behaviour objects into a single shared "
                << "library: " << errorMessage);
    m_state = MergingState::Failed;
}

void BehaviourManager::CompileMergedLibrary()
{
    Debug_Log("CompileBehavioursLibrary();");

    List<String> behaviourObjects =
            BehaviourManager::GetBehavioursObjectsFilepathsList();
    String libOutputFilepath =
            Persistence::GetProjectLibsRootAbs() + "/Behaviours.so";
    libOutputFilepath += "." + std::to_string(Time::GetNow()) + ".1.1";

    typedef SystemUtils::CompilationFlags CLFlags;
    bool successCompiling; String output;
    SystemUtils::Compile(behaviourObjects, libOutputFilepath,
                         &successCompiling, &output,
                           CLFlags::AddEngineObjectFiles  |
                           CLFlags::AddProjectObjectFiles |
                           CLFlags::ProduceSharedLib);
    if (successCompiling)
    {
        OnMergedLibraryCompiled(libOutputFilepath.ToQString(),
                                output.ToQString());
    }
    else
    {
        OnMergedLibraryCompilationFailed(output.ToQString());
    }

    QObject::disconnect(&m_mergeObjectsThread, SIGNAL(started()),
                        this, SLOT(CompileMergedLibrary()));
    m_mergeObjectsThread.quit();
}
