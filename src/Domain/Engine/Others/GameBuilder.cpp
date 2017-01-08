#include "GameBuilder.h"

#include "Debug.h"
#include "Dialog.h"
#include "Project.h"
#include "Persistence.h"
#include "StringUtils.h"
#include "SystemUtils.h"
#include "ProjectManager.h"
#include "BehaviourManager.h"

BuildGameThread GameBuilder::buildThread;

void GameBuilder::BuildGame(bool runGame)
{
    if (GameBuilder::buildThread.isRunning())
    {
        Debug_Status("Game is already being built", 5.0f);
    }
    else
    {
        // Compile!
        const String defaultOutputDirectory = Persistence::c_ProjectRootAbsolute;
        const String projectName = ProjectManager::GetCurrentProject()->GetProjectName();
        String outputFilepath =
            Dialog::GetSaveFilename("Choose the file where you want to create your game",
                                    "exe",
                                    defaultOutputDirectory,
                                    projectName + ".exe");

        ASSERT(!outputFilepath.Empty());

        String outputDir = Persistence::GetDir(outputFilepath);
        bool createdDataDir = GameBuilder::CreateDataDirectory(outputDir);
        ASSERT(createdDataDir, "Could not create data directory");

        outputFilepath = Persistence::AppendExtension(outputFilepath, "exe");
        GameBuilder::buildThread.m_outputFilepath = outputFilepath;

        Debug_Status("Building game...", 5.0f);
        GameBuilder::buildThread.m_runGameAfterBuild = runGame;
        GameBuilder::buildThread.start();
    }
}

bool GameBuilder::CreateDataDirectory(const String &parentDir)
{
    String dataDir = parentDir + "/GameData";
    if (!Persistence::CreateDirectory(dataDir)) { return false; }

    // Copy the Engine Assets in the GameData directory
    if (!Persistence::DuplicateDir(Persistence::c_EngineAssetsRootAbsolute,
                                   dataDir + "/EngineAssets"))
    {
        return false;
    }

    // Copy the Project Assets in the GameData directory
    if (!Persistence::DuplicateDir(Persistence::c_ProjectAssetsRootAbsolute,
                                   dataDir + "/Assets"))
    {
        return false;
    }

    // Create the project of the Game
    Project *gameProject =
            ProjectManager::CreateNewProjectFileOnly(dataDir + "/Game.bproject");
    if (!gameProject) { return false; }

    /*
    String projFilepath = proj->GetProjectFileFilepath();
    if (!Persistence::DuplicateFile(projFilepath, dataDir + "/Game.bproject"))
    {
        return false;
    }
    */

    // Compile the behaviours and save them so the Game can load them instantly,
    // and doesn't need to compile them
    List<String> behaviourFilepaths =
            Persistence::GetFiles(Persistence::c_ProjectAssetsRootAbsolute,
                                  true, { "*.cpp" });
    for (const String &behaviourFilepath : behaviourFilepaths)
    {
        String compiledLibFilepath =
                SystemUtils::CompileToSharedObject(behaviourFilepath, false);
        if (compiledLibFilepath.Empty())
        {
            Debug_Error("Failed to compile " << behaviourFilepath);
            return false;
        }

        String gameLibFilepath = compiledLibFilepath;
        gameLibFilepath.Replace(Persistence::c_ProjectAssetsRootAbsolute,
                               dataDir + "/Assets");
        String gameLibFilepathWithoutTimestamp =
                Persistence::GetDir(gameLibFilepath) + "/" +
                Persistence::GetFileName(gameLibFilepath) + ".so";

        // Add the random project Id, to avoid library(*.so) caching from
        // the operative system (it happens, yes)
        String randomProjectId = gameProject->GetProjectRandomId();
        Debug_Log(randomProjectId);
        String gameLibWithRandomProjectId =
                gameLibFilepathWithoutTimestamp + "." + randomProjectId;
        Persistence::Rename(compiledLibFilepath, gameLibWithRandomProjectId);
    }
}

void BuildGameThread::run()
{
    String output = "";
    String cmd = Persistence::c_EngineRootAbsolute + "/scripts/compile.sh GAME";

    const String initialOutputDir = Persistence::c_EngineRootAbsolute + "/bin/Game.exe";
    Persistence::Remove(initialOutputDir);

    bool ok = false;
    SystemUtils::System(cmd.ToCString(), &output, &ok);
    ok = ok && Persistence::ExistsFile(initialOutputDir);
    if (ok)
    {
        //Debug_Status("Game has been built!", 5.0f);
        Persistence::Remove(m_outputFilepath);
        Persistence::Move(initialOutputDir, m_outputFilepath);
        if (m_runGameAfterBuild)
        {
            //Debug_Status("Running Game...", 0.0f);
            SystemUtils::SystemBackground(m_outputFilepath); // Execute game
            //Debug_Status("Game is running!", 0.0f);
        }
    }
    else
    {
        Debug_Error(output);
    }
}
