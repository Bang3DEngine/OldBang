#ifndef GAMEBUILDER_H
#define GAMEBUILDER_H

#include <QObject>

#include "Bang/Path.h"
#include "Bang/BinType.h"

FORWARD class Project;

class GameBuilder : public QObject
{
    Q_OBJECT

public:
    static void BuildGame(const Project *project,
                          const Path &outputExecutableFilepath,
                          BinType binType,
                          bool compileBehaviours = true);

    GameBuilder() = delete;

private:
    static bool     CompileGameExecutable(BinType binType);
    static bool     CreateDataDirectory(const Path &executableDir);
    static Project* CreateGameProject(const Path &executableDir);
    static bool     CompileBehaviours(const Path &executableDir,
                                      BinType binType);
};

#endif // GAMEBUILDER_H
