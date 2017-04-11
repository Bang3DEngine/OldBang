#include "Bang/GameBuilderJob.h"

#include "Bang/Debug.h"
#include "Bang/IO.h"
#include "Bang/GameBuilder.h"
#include "Bang/GameBuildDialog.h"

#define CHECK_CANCEL if (m_canceled) \
                     { QThread::currentThread()->exit(0); return; }

GameBuilderJob::GameBuilderJob()
{
}

void GameBuilderJob::BuildGame()
{
    ENSURE(!m_executableFilepath.Empty());
    GameBuilder *gb = GameBuilder::GetInstance(); ENSURE(gb);

    String executableDir = IO::GetDir(m_executableFilepath);

    emit NotifyPercent(0.0f);

    CHECK_CANCEL;

    emit NotifyMessage("Compiling the game executable...");
    if (!gb->CompileGameExecutable())
    {
        emit NotifyGameBuildingHasFailed("Could not compile game executable");
        return;
    }
    emit NotifyPercent(0.5f);

    CHECK_CANCEL;

    emit NotifyMessage("Copying assets into data directory...");
    if (!gb->CreateDataDirectory(executableDir))
    {
        emit NotifyGameBuildingHasFailed("Could not create data directory");
        return;
    }
    emit NotifyPercent(0.75f);

    CHECK_CANCEL;

    emit NotifyMessage("Creating Game project file...");
    Project *gameProject = gb->CreateGameProject(executableDir);
    if (!gameProject)
    {
        emit NotifyGameBuildingHasFailed("Could not create game project file");
        return;
    }

    CHECK_CANCEL;

    emit NotifyMessage("Compiling behaviours...");
    if (!gb->CompileBehaviours(executableDir, gameProject, &m_canceled))
    {
        emit NotifyGameBuildingHasFailed("Could not compile the behaviours");
        return;
    }
    emit NotifyPercent(0.95f);

    CHECK_CANCEL;

    emit NotifyMessage("Moving the executable into the desired location...");
    const String c_initialOutputDir = IO::GetEngineRootAbs() +
                                      "/bin/Game.exe";
    IO::Remove(m_executableFilepath);
    IO::Move(c_initialOutputDir, m_executableFilepath);

    emit NotifyPercent(1.0f);

    emit NotifyGameHasBeenBuilt();
}

void GameBuilderJob::OnGameBuildingCanceled()
{
    m_canceled = true;
}
