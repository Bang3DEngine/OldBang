#ifndef GAMEBUILDER_H
#define GAMEBUILDER_H

#include <QThread>
#include <QProgressDialog>

#include "String.h"
#include "GameBuilderJob.h"

class Project;
class GameBuildDialog;
class GameBuilder : public QObject
{
    Q_OBJECT

public:
    static GameBuilder* GetInstance();

    void BuildGame(bool runGame = false);
    void OnGameBuildingHasBeenCanceled();

public slots:
    void OnGameHasBeenBuilt();
    void OnGameBuildingHasFailed();

signals:
    void NotifyGameBuildingCanceled();

private:
    GameBuilder();
    virtual ~GameBuilder();

    static GameBuilder *s_instance;

    QThread *m_gameBuilderThread = nullptr;
    GameBuilderJob *m_gameBuilderJob = nullptr;
    GameBuildDialog *m_gameBuildDialog = nullptr;
    String m_latestGameExecutableFilepath = "";

    String AskForExecutableFilepath();
    bool     CompileGameExecutable();
    bool     CreateDataDirectory(const String &executableDir);
    Project* CreateGameProject(const String &executableDir);
    bool     CompileBehaviours(const String &executableDir,
                               Project *GameProject,
                               bool *cancel);
    void RemoveLatestGameBuild();

    GameBuildDialog *GetGameBuildDialog();


    friend class GameBuilderJob;
};

#endif // GAMEBUILDER_H
