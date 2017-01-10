#include "GameBuilder.h"

#include "Debug.h"
#include "Dialog.h"
#include "Project.h"
#include "Persistence.h"
#include "StringUtils.h"
#include "SystemUtils.h"
#include "EditorWindow.h"
#include "ProjectManager.h"
#include "GameBuildDialog.h"
#include "BehaviourManager.h"

GameBuilder *GameBuilder::s_instance = nullptr;

GameBuilder::GameBuilder()
{
}

GameBuilder::~GameBuilder()
{
}

GameBuilder *GameBuilder::GetInstance()
{
    if (!GameBuilder::s_instance)
    {
        GameBuilder::s_instance = new GameBuilder();
    }
    return GameBuilder::s_instance;
}

String GameBuilder::AskForExecutableFilepath()
{
    // Get the executable output filepath
    const String defaultOutputDirectory = Persistence::c_ProjectRootAbsolute;
    const String projectName = ProjectManager::GetCurrentProject()->GetProjectName();
    Debug_Log("HIIIII");
    String executableFilepath =
        Dialog::GetSaveFilename("Choose the file where you want to create your game",
                                "exe",
                                defaultOutputDirectory,
                                projectName + ".exe");

    if (!executableFilepath.Empty())
    {
        executableFilepath =
                Persistence::AppendExtension(executableFilepath, "exe");
    }
    return executableFilepath;
}

void GameBuilder::BuildGame(bool runGame)
{
    if (m_buildThread.isRunning())
    {
        Debug_Status("Game is already being built", 5.0f);
    }
    else
    {
        // First ask for the executable output file (in the main thread)
        String executableFilepath = AskForExecutableFilepath();
        ASSERT(!executableFilepath.Empty());

        // Create the progress window
        if (m_gameBuildDialog) { delete m_gameBuildDialog; }
        m_gameBuildDialog = new GameBuildDialog();

        // Create and start the building thread
        QObject::connect(&m_buildThread, SIGNAL(NotifyPercent(float)),
                          m_gameBuildDialog, SLOT(SetPercent(float)));
        QObject::connect(&m_buildThread, SIGNAL(NotifyMessage(String)),
                          m_gameBuildDialog, SLOT(SetMessage(String)));
        m_buildThread.m_executableFilepath = executableFilepath;
        m_buildThread.m_runGameAfterBuild  = runGame;
        m_buildThread.start();
    }
}

bool GameBuilder::CompileGameExecutable(const String &gameExecutableFilepath)
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
        Persistence::Remove(gameExecutableFilepath);
        Persistence::Move(initialOutputDir, gameExecutableFilepath);
        //Debug_Status("Game has been built!", 5.0f);
        /*
        if (m_runGameAfterBuild)
        {
            //Debug_Status("Running Game...", 0.0f);
            SystemUtils::SystemBackground(m_outputFilepath); // Execute game
            //Debug_Status("Game is running!", 0.0f);
        }
        */
        return true;
    }
    else
    {
        Debug_Error(output);
        return false;
    }
}

bool GameBuilder::CreateDataDirectory(const String &executableDir)
{
    String dataDir = executableDir + "/GameData";
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

    return true;
}

Project* GameBuilder::CreateGameProject(const String &executableDir)
{
    String dataDir = executableDir + "/GameData";
    Project *gameProject =
            ProjectManager::CreateNewProjectFileOnly(dataDir + "/Game.bproject");
    return gameProject;
}

bool GameBuilder::CompileBehaviours(const String &executableDir,
                                    Project *gameProject)
{
    String dataDir = executableDir + "/GameData";

    // Compile the behaviours and save them so the Game can
    // load them instantly and doesn't need to compile them
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

        // Add the random project Id to the name, to avoid library(*.so)
        // caching from by operative system (it happens, yes)
        String randomProjectId = gameProject->GetProjectRandomId();
        String gameLibWithRandomProjectId =
                gameLibFilepathWithoutTimestamp + "." + randomProjectId;
        Persistence::Rename(compiledLibFilepath, gameLibWithRandomProjectId);
    }

    return true;
}

GameBuildDialog *GameBuilder::GetGameBuildDialog()
{
    return m_gameBuildDialog;
}
