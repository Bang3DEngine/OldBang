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
        GameBuilder::buildThread.runGameAfterBuild = runGame;
        GameBuilder::buildThread.start();
    }
}

void BuildGameThread::run()
{
    String output = "";
    String cmd = Persistence::c_EngineRootAbsolute + "/scripts/compile.sh GAME";


    // Compile!
    const String defaultOutputDirectory = Persistence::c_ProjectRootAbsolute;
    const String projectName = ProjectManager::GetCurrentProject()->GetProjectName();
    String outputFilepath =
        Dialog::GetSaveFilename("Choose the file where you want to create your game",
                                "*",
                                defaultOutputDirectory,
                                defaultOutputDirectory + "/" + projectName + ".exe");

    ASSERT(!outputFilepath.Empty());

    Debug_Log(outputFilepath);

    outputFilepath = Persistence::AppendExtension(outputFilepath, "exe");
    const String initialOutputDir = Persistence::c_EngineRootAbsolute + "/bin/Game.exe";
    Persistence::Remove(initialOutputDir);

    bool ok = false;
    SystemUtils::System(cmd.ToCString(), &output, &ok);
    ok = ok && Persistence::ExistsFile(initialOutputDir);
    if (!ok)
    {
        Debug_Error(output);
    }
    else
    {
        Debug_Status("Game has been built!", 5.0f);
    }

    if (ok && runGameAfterBuild)
    {
        String output = "";
        Debug_Status("Running Game...", 0.0f);
        SystemUtils::SystemBackground("./Game.exe");
        Debug_Status("Game is running!", 0.0f);
    }
}
