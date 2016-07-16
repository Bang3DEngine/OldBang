#include "GameBuilder.h"

BuildGameThread GameBuilder::buildThread;

void GameBuilder::BuildGame(const std::string &absoluteDir)
{
    GameBuilder::buildThread.runGame = false;
    GameBuilder::buildThread.start();
}

void GameBuilder::BuildAndRunGame(const std::string &absoluteDir)
{
    GameBuilder::buildThread.runGame = true;
    GameBuilder::buildThread.start();
}


void BuildGameThread::run()
{
    bool ok = false;
    std::string output = "";
    SystemUtils::System("qmake \"BUILD_MODE=GAME\" && make", output, ok);

    if(!ok)
    {
        Logger_Error(output);
    }
    else
    {
        Logger_Log("Game has been built!");
    }

    if(runGame)
    {
        bool ok = false;
        std::string output = "";
        Logger_Log("Running Game...");
        SystemUtils::System("./Game.exe &", output, ok);

        if(!ok)
        {
            Logger_Error(output);
        }
        else
        {
            Logger_Log("Game is running!");
        }
    }
}
