#include "GameBuilder.h"

BuildGameThread GameBuilder::buildThread;

void GameBuilder::BuildGame(const String &absoluteDir)
{
    GameBuilder::buildThread.runGameAfterBuild = false;
    GameBuilder::buildThread.start();
}

void GameBuilder::BuildAndRunGame(const String &absoluteDir)
{
    GameBuilder::buildThread.runGameAfterBuild = true;
    GameBuilder::buildThread.start();
}


void BuildGameThread::run()
{
    bool ok = false;
    String output = "";
    SystemUtils::System("qmake \"BUILD_MODE=GAME\" && make -j8", &output, &ok);

    if (!ok)
    {
        Debug_Error(output);
    }
    else
    {
        Debug_Log("Game has been built!");
    }

    if (ok && runGameAfterBuild)
    {
        ok = false;
        String output = "";
        Debug_Log("Running Game...");
        SystemUtils::System("./Game.exe &", &output, &ok);

        if (!ok)
        {
            Debug_Error(output);
        }
        else
        {
            Debug_Log("Game is running!");
        }
    }
}
