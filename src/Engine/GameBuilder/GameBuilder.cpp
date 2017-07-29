#include "Bang/GameBuilder.h"

#include <QProgressDialog>

#include "Bang/File.h"
#include "Bang/Time.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Project.h"
#include "Bang/Extensions.h"
#include "Bang/SystemUtils.h"
#include "Bang/ProjectManager.h"
#include "Bang/BangPreprocessor.h"
#include "Bang/BehaviourManager.h"

void GameBuilder::BuildGame(const Project *project,
                            const Path &outputExecutableFilepath,
                            BinType binaryType,
                            bool compileBehaviours)
{
    Path executableDir = outputExecutableFilepath.GetDirectory();

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

    if (compileBehaviours)
    {
        Debug_Log("Compiling behaviours...");
        if (!GameBuilder::CompileBehaviours(executableDir, binaryType))
        {
            Debug_Error("Could not compile the behaviours");
            return;
        }
    }

    Debug_Log("Moving the executable to '" <<outputExecutableFilepath  << "'...");
    const Path c_initialOutputDir = Paths::GameExecutableOutputFile(binaryType);
    File::Remove(outputExecutableFilepath);
    File::Move(c_initialOutputDir, outputExecutableFilepath);
}

bool GameBuilder::CompileGameExecutable(BinType binaryType)
{
    List<Path> sceneFiles = Paths::ProjectAssets().FindFiles(
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
                                    BinType binType)
{
    // Create Libraries directory
    Path libsDir = Path(executableDir).Append("GameData").Append("Libraries");
    File::CreateDirectory(libsDir);

    // Compile every behaviour into its .o
    List<Path> behavioursSourceFiles =
            Paths::ProjectAssets().FindFiles(true,
                                             Extensions::GetSourceFileList());

    // Preprocess behaviours before
    for (const Path &behaviourSourcePath : behavioursSourceFiles)
    {
        Path behaviourHeader = behaviourSourcePath.ChangeExtension("h");
        BangPreprocessor::Preprocess(behaviourHeader);
    }

    // Compile
    for (const Path &behaviourSourcePath : behavioursSourceFiles)
    {
        Path outputObjPath = libsDir.Append(behaviourSourcePath.GetName())
                                    .AppendExtension("o");

        Debug_Log("Compiling '" << behaviourSourcePath << "' into '" <<
                  outputObjPath << "'...");
        Compiler::Result res =
                BehaviourManager::CompileBehaviourObject(behaviourSourcePath,
                                                         outputObjPath,
                                                         binType);

        if (!res.success) { Debug_Error(res.output); return false; }
    }
    //

    // Merge into .so
    List<Path> behaviourObjectsPaths = libsDir.FindFiles(false, {"*.o"});
    Path outputLibPath =
                libsDir.Append("Behaviours")
                       .AppendExtension("so")
                       .AppendExtension( String::ToString(Time::GetNow()) );
    Debug_Log("Merging behaviour objects into '" << outputLibPath << "'...");

    Compiler::Result res =
            BehaviourManager::MergeBehaviourObjects(outputLibPath,
                                                    behaviourObjectsPaths,
                                                    binType);
    if (!res.success) { Debug_Error(res.output); return false; }
    //

    return true;
}
