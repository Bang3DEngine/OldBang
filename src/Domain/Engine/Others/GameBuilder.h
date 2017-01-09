#ifndef GAMEBUILDER_H
#define GAMEBUILDER_H

#include <QThread>

#include "String.h"

class BuildGameThread : public QThread
{
    Q_OBJECT
    public:
        String m_outputFilepath = "";
        bool m_runGameAfterBuild = false;

    protected:
        void run() override;
        void BuildGame();

    friend class GameBuilder;
};

class GameBuilder
{
public:
    static void BuildGame(bool runGame = false);

private:
    GameBuilder() {}

    static BuildGameThread buildThread;
    static bool CreateDataDirectory(const String &parentDir);
};

#endif // GAMEBUILDER_H
