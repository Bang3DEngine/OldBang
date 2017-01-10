#ifndef GAMEBUILDER_H
#define GAMEBUILDER_H

#include <QThread>
#include <QProgressDialog>

#include "String.h"
#include "GameBuilderThread.h"

class Project;
class GameBuildDialog;
class GameBuilder
{
public:
    static GameBuilder* GetInstance();

    void BuildGame(bool runGame = false);

private:
    GameBuilder();
    virtual ~GameBuilder();

    static GameBuilder *s_instance;

    GameBuilderThread m_buildThread;
    GameBuildDialog *m_gameBuildDialog = nullptr;

    String AskForExecutableFilepath();
    bool     CompileGameExecutable(const String &gameExecutableFilepath);
    bool     CreateDataDirectory(const String &executableDir);
    Project* CreateGameProject(const String &executableDir);
    bool     CompileBehaviours(const String &executableDir,
                               Project *GameProject);

    GameBuildDialog *GetGameBuildDialog();

    friend class GameBuilderThread;
};

#endif // GAMEBUILDER_H
