#ifndef GAMEBUILDER_H
#define GAMEBUILDER_H

#include <QThread>
#include <QProgressDialog>
#include "Bang/WinUndef.h"

#include "Bang/String.h"
#include "Bang/GameBuilderJob.h"

class Project;
class GameBuildDialog;
class GameBuilder : public QObject
{
    Q_OBJECT

public:
    static GameBuilder* GetInstance();

    void BuildGame(bool runGame = false);

public slots:
    void OnGameHasBeenBuilt();
    void OnGameBuildingHasBeenCanceled();
    void OnGameBuildingHasFailed(const QString &errorMsg);
    void OnDialogError(const QString &title, const QString &msg);

signals:
    void NotifyGameBuildingCanceled();
    void DialogError(const QString &title, const QString &msg);

private:
    GameBuilder();
    virtual ~GameBuilder();

    static GameBuilder *s_instance;

    bool m_runGameAfterBuild = false;
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
