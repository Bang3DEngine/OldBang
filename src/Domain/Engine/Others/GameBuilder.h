#ifndef GAMEBUILDER_H
#define GAMEBUILDER_H

#include <QThread>

#include "Debug.h"
#include "StringUtils.h"
#include "SystemUtils.h"

class BuildGameThread : public QThread
{
    Q_OBJECT
    public:
        bool runGameAfterBuild = false;

    protected:
        void run() override;
};

class GameBuilder
{
private:
    static BuildGameThread buildThread;

    GameBuilder() {}

public:
    static void BuildGame(const String &absoluteDir, bool runGame = false);
};

#endif // GAMEBUILDER_H
