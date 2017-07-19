#include "Bang/GameBuilder.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Project.h"
#include "Bang/Extensions.h"
#include "Bang/SystemUtils.h"
#include "Bang/ProjectManager.h"
// #include "Bang/BehaviourManager.h"

void GameBuilder::BuildGame(const Project *project,
                            const Path &executablePath,
                            BinType binaryType)
{
    Path executableDir = executablePath.GetDirectory();

    Debug_Log("Compiling the game executable...");
    if (!GameBuilder::CompileGameExecutable(binaryType))
    {
        Debug_Error("Could not compile game executable");
        return;
    }

    Debug_Log("Copying assets into data directory...");
    if (!GameBuilder::CreateDataDirectory(executableDir))
    {
        Debug_Error("Could not create data directory");
        return;
    }

    Debug_Log("Creating Game project file...");
    Project *gameProject = GameBuilder::CreateGameProject(executableDir);
    if (!gameProject)
    {
        Debug_Error("Could not create game project file");
        return;
    }

    Debug_Log("Compiling behaviours...");
    bool canceled = false;
    if (!GameBuilder::CompileBehaviours(executableDir, &canceled))
    {
        Debug_Error("Could not compile the behaviours");
        return;
    }
    if (canceled) { return; }

    Debug_Error("Moving the executable into the desired location...");
    const Path c_initialOutputDir = Paths::GameExecutableOutputFile(binaryType);
    File::Remove(executablePath);
    File::Move(c_initialOutputDir, executablePath);
}

bool GameBuilder::CompileGameExecutable(BinType binaryType)
{
    List<Path> sceneFiles = Paths::ProjectAssets().GetFiles(
                                true, {"*." + Extensions::Get<Scene>()});
    if (sceneFiles.IsEmpty())
    {
        Debug_Error("Please save at least one scene in the \
                     Assets directory to build the game");
        return false;
    }

    const Path gameOutputFilepath = Paths::GameExecutableOutputFile(binaryType);
    File::Remove(gameOutputFilepath);

    bool ok = false;
    String output = "";
    String debugRelease = (binaryType == BinType::Debug) ? "DEBUG" : "RELEASE";

    String cmd = Paths::Engine() + "/scripts/compile.sh";
    SystemUtils::System(cmd.ToCString(), {debugRelease}, &output, &ok);
    Debug_Log(cmd);
    ok = ok && gameOutputFilepath.IsFile();
    if (!ok)
    {
        Debug_Error(output);
        return false;
    }
    return true;
}

bool GameBuilder::CreateDataDirectory(const Path &executableDir)
{
    Path dataDir = executableDir.Append("GameData");
    File::Remove(dataDir);
    if (!File::CreateDirectory(dataDir)) { return false; }

    // Copy the Engine Assets in the GameData directory
    Path dataResDir = dataDir.Append("res");
    if (!File::CreateDirectory(dataResDir)) { return false; }
    if (!File::DuplicateDir(Paths::EngineAssets(),
                            dataResDir.Append("EngineAssets")))
    {
        return false;
    }

    // Copy the Project Assets in the GameData directory
    if (!File::DuplicateDir(Paths::ProjectAssets(), dataDir.Append("Assets")))
    {
        return false;
    }

    return true;
}

Project *GameBuilder::CreateGameProject(const Path &executableDir)
{
    String projectFile = executableDir + "/GameData/Game.bproject";
    ProjectManager pm;
    return pm.CreateNewProjectFileOnly( Path(projectFile ) );
}

bool GameBuilder::CompileBehaviours(const Path &executableDir,
                                    bool *cancel)
{
    if (*cancel) { return true; }

    Path libsDir = Path(executableDir).Append("GameData").Append("Libraries");
    File::CreateDirectory(libsDir);
    bool success = true;
    // bool success = BehaviourManager::PrepareBehavioursLibrary(true,
    //                                                           libsDir,
    //                                                           cancel);
    return success;
}
