#include <QTextStream>
#include <QStyleFactory>
#include "Bang/WinUndef.h"

#include "Bang/Path.h"
#include "Bang/EditorScene.h"
#include "Bang/EditorWindow.h"
#include "Bang/SingletonManager.h"
#include "Bang/EditorApplication.h"
#include "Bang/SelectProjectWindow.h"
#include "Bang/EditorProjectManager.h"

void LoadStylesheet(QApplication *app)
{
    // Load dark Window theme
    Path stylesheetPath = Paths::EngineAssets().Append("/qdarkstyle/style.qss");
    QFile f(stylesheetPath .GetAbsolute().ToQString());
    if (!f.exists())
    {
        Debug_Error("Unable to set stylesheet, not found.");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        app->setStyleSheet(ts.readAll());
    }

    // Set Font
    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPointSize(8);
    app->setFont(font);
}

void InitSingletonManager()
{
    SingletonManager::InitSingletonManagerFromMainBinary();
    Time::InitFromMainBinary();
    Input::InitFromMainBinary();
}

int main(int argc, char **argv)
{
    EditorApplication app(argc, argv);

    InitSingletonManager();

    Path executableDir(Application::applicationDirPath());
    Paths::SetEngineBinaryDir(executableDir);

    // Init engine paths, by looking at executable location:
    Path engineRootDir = executableDir.GetDirectory().GetDirectory();
    Paths::SetEnginePath(engineRootDir);

    LoadStylesheet(&app);

    // Select project
    Path loadedProjectFilepath;
    QMainWindow *selectProjectWindow = new QMainWindow();
    loadedProjectFilepath =
            SelectProjectWindow::ExecAndGetProjectFilepath(selectProjectWindow,
                                                           &app);
    if (loadedProjectFilepath.IsEmpty())
	{
		Debug_Error("The project filepath has not been specified.");
		return 1;
	}

    app.InitManagers();

    QMainWindow *qWindow = new QMainWindow();
    EditorWindow::InitFromMainBinary(qWindow, &app);

    EditorProjectManager pm;
    pm.OpenProject(loadedProjectFilepath);

    app.MainLoop();

    return 0;
}
