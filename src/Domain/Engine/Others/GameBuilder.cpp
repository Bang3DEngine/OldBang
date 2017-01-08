#include "GameBuilder.h"

#include "Debug.h"
#include "Dialog.h"
#include "Project.h"
#include "Persistence.h"
#include "StringUtils.h"
#include "SystemUtils.h"
#include "ProjectManager.h"

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
        Debug_Log(defaultOutputDirectory);
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

    Project *proj = ProjectManager::GetCurrentProject();
    String projFilepath = proj->GetProjectFileFilepath();
    if (!Persistence::DuplicateFile(projFilepath, dataDir + "/Game.bproject"))
    {
        return false;
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
