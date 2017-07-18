#include "Bang/BehaviourManager.h"

#include <QLibrary>

#include "Bang/Time.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Project.h"
#include "Bang/Behaviour.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/ProjectManager.h"
#include "Bang/CodePreprocessor.h"

BehaviourManager::BehaviourManager()
{
    BangCompiler::Init();
}

BehaviourManager *BehaviourManager::GetInstance()
{
    Application *app = Application::GetInstance();
    return app ? app->m_behaviourManager : nullptr;
}

QLibrary *BehaviourManager::GetBehavioursMergedLibrary()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    return bm->m_behavioursLibrary;
}

bool BehaviourManager::PrepareBehavioursLibrary(bool forGame,
                                                const Path& outputLibDir,
                                                bool *stopFlag)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();

    // In game, the library is compiled from before.
    if (!BehaviourManager::GetBehavioursMergedLibrary())
    {
        Project *project = ProjectManager::GetCurrentProject();
        Path libDir = Paths::ProjectLibrariesDir();
        String projId = project->GetProjectRandomId();
        String libFilepath = "Behaviours.so." + projId + ".1.1";
        Path libOutput = libDir.Append(libFilepath);
        BangCompiler::Result result;
        result.compileJob.outputFile = libOutput;
        result.success = true;
        bm->OnMergingLibraryFinished(result);
    }
    return true;
}

bool BehaviourManager::StartMergingBehavioursObjects(bool forGame,
                                                     const Path& outputLibDir)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    if (!bm->GetStatus().AllBehavioursReady(outputLibDir)) { return false; }
    if (bm->GetStatus().IsMerging()) { return true; }

    List<Path> objsPaths = outputLibDir.GetFiles(false, {"*.o"});
    QSlot callbackSlot = SLOT(OnMergingLibraryFinished(BangCompiler::Result));
    bool mergeStarted = BangCompiler::MergeBehaviourLibrariesAsync(
                                                                objsPaths,
                                                                outputLibDir,
                                                                forGame,
                                                                bm,
                                                                callbackSlot);
    if (mergeStarted) { bm->m_status.OnMergingStarted(); }
    return mergeStarted;
}

void BehaviourManager::StartCompilingAllBehaviourObjects(bool forGame,
                                                         const Path& objectsDir)
{
    List<Path> allBehaviourSources = Paths::GetBehavioursSourcesFilepaths();
    for (const Path &behFilepath : allBehaviourSources)
    {
        BehaviourManager *bm = BehaviourManager::GetInstance();
        if (!bm->GetStatus().IsReady(behFilepath, objectsDir) &&
            !bm->GetStatus().IsBeingCompiled(behFilepath)     &&
            !bm->GetStatus().HasFailed(behFilepath))
        {
            BehaviourManager::StartCompilingBehaviourObject(behFilepath,
                                                            objectsDir,
                                                            forGame);
        }
    }
}

void BehaviourManager::StartCompilingBehaviourObject(const Path &behFilepath,
                                                     const Path& objectsDir,
                                                     bool forGame)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();

    QSlot callbackSlot =
              SLOT(OnBehaviourObjectCompilationFinished(BangCompiler::Result));
    bool compileStarted = BangCompiler::CompileBehaviourObjectAsync(
                                            behFilepath,
                                            objectsDir,
                                            forGame,
                                            bm,
                                            callbackSlot);
    if (compileStarted)
    {
        Debug_Status("Compiling " << behFilepath, 3.0f);
        bm->m_status.OnBehaviourStartedCompiling(behFilepath);
    }
}

const BehaviourManagerStatus &BehaviourManager::GetStatus()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    return bm->m_status;
}

void BehaviourManager::OnBehaviourObjectCompilationFinished(
                                            const BangCompiler::Result &result)
{
    if (result.success)
    {
        const Path &behFilepath = result.compileJob.inputFiles.Front();
        m_status.OnBehaviourSuccessCompiling(behFilepath);
        if (!result.output.Empty()) { Debug_Warn(result.output); }
    }
    else
    {
        m_status.OnBehaviourObjectCompilationFailed(
                    result.compileJob.inputFiles.Front(), result.output);
    }
}

void BehaviourManager::OnMergingLibraryFinished(
                                        const BangCompiler::Result &result)
{
    if (result.success)
    {
        if (!result.output.Empty()) { Debug_Warn(result.output); }

        if (!result.forGame) // Load library
        {
            String behaviourLibPath = result.compileJob.outputFile.GetAbsolute();
            QLibrary *behLib = new QLibrary(behaviourLibPath.ToQString());
            behLib->setLoadHints(QLibrary::LoadHint::ResolveAllSymbolsHint);
            bool success = behLib->load();
            m_behavioursLibrary = success ? behLib : nullptr;

            if (!success)
            {
                Debug_Error("There was an error when loading the library '" <<
                             behaviourLibPath << "': " << behLib->errorString());
            }
        }
    }
    else
    {
        Debug_Error("Error while merging behaviour objects into a single shared "
                    << "library: " << result.output);
    }

    m_status.OnMergingFinished(result.success);
}
