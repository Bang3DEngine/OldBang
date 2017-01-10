#include "GameBuilderThread.h"

#include "Debug.h"
#include "Persistence.h"
#include "GameBuilder.h"
#include "GameBuildDialog.h"

GameBuilderThread::GameBuilderThread()
{
}

void GameBuilderThread::run()
{
    BuildGame();

    // Close the GameBuildDialog
    GameBuilder *gb = GameBuilder::GetInstance(); ASSERT(gb);
    GameBuildDialog *gbDialog = gb->GetGameBuildDialog();
    gbDialog->close();
}

void GameBuilderThread::BuildGame()
{
    ASSERT(!m_executableFilepath.Empty());

    GameBuilder *gb = GameBuilder::GetInstance(); ASSERT(gb);
    GameBuildDialog *gbDialog = gb->GetGameBuildDialog();

    String executableDir = Persistence::GetDir(m_executableFilepath);

    emit NotifyPercent(0.0f);

    emit NotifyMessage("Compiling the game executable...");
    ASSERT( gb->CompileGameExecutable(m_executableFilepath),
            "Could not compile game executable" );
    emit NotifyPercent(0.5f);

    emit NotifyMessage("Copying assets into data directory...");
    ASSERT( gb->CreateDataDirectory(executableDir),
            "Could not create data directory" );
    emit NotifyPercent(0.75f);

    emit NotifyMessage("Creating Game project file...");
    Project *gameProject = gb->CreateGameProject(executableDir);
    ASSERT( gameProject, "Could not create game project file" );

    emit NotifyMessage("Compiling behaviours...");
    ASSERT( gb->CompileBehaviours(executableDir, gameProject),
            "Could not compile the behaviours" );

    emit NotifyPercent(1.0f);

    disconnect(this, SIGNAL(NotifyPercent(float)),
               gbDialog, SLOT(SetPercent(float)));
    disconnect(this, SIGNAL(NotifyMessage(String)),
               gbDialog, SLOT(SetMessage(String)));
}
