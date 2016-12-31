#include "GameBuilder.h"


#include "Debug.h"
#include "StringUtils.h"
#include "SystemUtils.h"

BuildGameThread GameBuilder::buildThread;

void GameBuilder::BuildGame(const String &absoluteDir, bool runGame)
{
    if (GameBuilder::buildThread.isRunning())
    {
        Debug_Status("Game is already being built");
    }
    else
    {
        GameBuilder::buildThread.runGameAfterBuild = runGame;
        GameBuilder::buildThread.start();
    }
}

void BuildGameThread::run()
{
    bool ok = false;
    String output = "";
    String cmd = "./compile.sh GAME";

    SystemUtils::System(cmd.ToCString(), &output, &ok);

    if (!ok)
    {
        Debug_Error(output);
    }
    else
    {
        Debug_Status("Game has been built!");
    }

    if (ok && runGameAfterBuild)
    {
        String output = "";
        Debug_Status("Running Game...");
        SystemUtils::SystemBackground("./Game.exe");
        Debug_Status("Game is running!");
    }
}
