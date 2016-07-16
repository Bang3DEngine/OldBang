#ifndef GAMEBUILDER_H
#define GAMEBUILDER_H

#include <QThread>

#include "Logger.h"
#include "StringUtils.h"
#include "SystemUtils.h"

class BuildGameThread : public QThread
{
    Q_OBJECT
    public:
        bool runGame = false;

    protected:
        void run() override;
};

class GameBuilder
{
private:
    static BuildGameThread buildThread;

    GameBuilder() {}

public:
    static void BuildGame(const std::string &absoluteDir);
    static void BuildAndRunGame(const std::string &absoluteDir);
};

#endif // GAMEBUILDER_H
