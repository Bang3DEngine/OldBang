#ifndef GAMEBUILDER_H
#define GAMEBUILDER_H

#include <QThread>
#include <QProgressDialog>

#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/BinType.h"

class Project;
class GameBuilder : public QObject
{
    Q_OBJECT

public:
    static void BuildGame(const Project *project,
                          const Path &executablePath,
                          BinType binType);

private:
    GameBuilder() = delete;

    static bool     CompileGameExecutable(BinType binType);
    static bool     CreateDataDirectory(const Path &executableDir);
    static Project* CreateGameProject(const Path &executableDir);
    static bool     CompileBehaviours(const Path &executableDir,
                                      bool *cancel);
};

#endif // GAMEBUILDER_H
