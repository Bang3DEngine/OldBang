#include "Bang/Path.h"
#include "Bang/GameWindow.h"
#include "Bang/Application.h"
#include "Bang/ProjectManager.h"
#include "Bang/BehaviourManager.h"
#include "Bang/SingletonManager.h"

void InitSingletonManager()
{
    SingletonManager::InitSingletonManagerFromMainBinary();
    Time::InitFromMainBinary();
    Input::InitFromMainBinary();
}

int main(int argc, char **argv)
{
    Application app(argc, argv);

    InitSingletonManager();

    Path executableDir(Application::applicationDirPath());
    Paths::SetEngineBinaryDir(executableDir);
    Paths::SetEnginePath(executableDir.Append("GameData"));

    Path loadedProjectFilepath = Path(executableDir + "/GameData/Game.bproject");
    if (loadedProjectFilepath.IsEmpty())
    {
        Debug_Error("Could not find GameData/Game.bproject !!!");
        return 1;
    }

    app.InitManagers();

    QMainWindow *qWindow = new QMainWindow();
    GameWindow::InitFromMainBinary(qWindow, &app);

    ProjectManager pm;
    pm.OpenProject(loadedProjectFilepath);
    BehaviourManager::PrepareBehavioursLibrary(true,
                                               Paths::ProjectLibrariesDir());

    app.MainLoop();

    return 0;
}
