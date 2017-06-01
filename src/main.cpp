#include "Bang/Array.h"

#include <iostream>

#include <QTextStream>
#include <QStyleFactory>
#include "Bang/WinUndef.h"

#include "Bang/glm/glm.hpp"

#include "Bang/Path.h"
#include "Bang/Array.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Input.h"
#include "Bang/Timer.h"
#include "Bang/Debug.h"
#include "Bang/Camera.h"
#include "Bang/Shader.h"
#include "Bang/Screen.h"
#include "Bang/String.h"
#include "Bang/XMLNode.h"
#include "Bang/AudioClip.h"
#include "Bang/Behaviour.h"
#include "Bang/Texture2D.h"
#include "Bang/XMLParser.h"
#include "Bang/GameObject.h"
#include "Bang/FileReader.h"
#include "Bang/Application.h"
#include "Bang/Framebuffer.h"
#include "Bang/SceneManager.h"
#include "Bang/MeshRenderer.h"
#include "Bang/ShaderProgram.h"
#include "Bang/ProjectManager.h"
#include "Bang/SingletonManager.h"

#ifdef BANG_EDITOR

#include "Bang/EditorScene.h"
#include "Bang/EditorWindow.h"
#include "Bang/SelectProjectWindow.h"

#else

#include "Bang/GameWindow.h"

#endif


void LoadStylesheet(QApplication *app)
{
    /*
    app->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    darkPalette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);

    app->setPalette(darkPalette); // Must go before setStylesheet
    app->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; \
                                   border: 1px solid white; } \
                        QLineEdit { border: 1px solid #666666; border-radius: 2px; \
                                    background-color: #222222; } \
                        QToolButton:checked { background-color: #111111; }");

                        */
    // Load dark Window theme
    QFile f((Paths::EngineAssets().Append("/qdarkstyle/style.qss"))
              .GetAbsolute().ToQString());
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

void InitEditorOrGame(QMainWindow *window, Application *app)
{
    #ifdef BANG_EDITOR
    EditorWindow::InitFromMainBinary(window, app);
    #else
    GameWindow::InitFromMainBinary(window, app);
    #endif

    Screen::GetInstance()->makeCurrent();
    Screen::GetInstance()->initializeGL();
}

#include "Bang/BangPreprocessor.h"
int main(int argc, char **argv)
{
    Application app(argc, argv);

    InitSingletonManager();

    Path executableDir(Application::applicationDirPath());

    // Init engine paths, by looking at executable location:
    #ifdef BANG_EDITOR
    Path engineRootDir = executableDir.GetDirectory();
    Paths::SetEnginePath(engineRootDir);
    #else
    Paths::SetEnginePath(executableDir.Append("GameData"));
    #endif

    LoadStylesheet(&app);

    Path loadedProjectFilepath;
    #ifdef BANG_EDITOR
    // Select project
    QMainWindow *selectProjectWindow = new QMainWindow();
    loadedProjectFilepath =
            SelectProjectWindow::ExecAndGetProjectFilepath(selectProjectWindow,
                                                           &app);
    #else
    loadedProjectFilepath = Path(executableDir + "/GameData/Game.bproject");
    #endif

    if (loadedProjectFilepath.IsEmpty())
	{
		Debug_Error("The project filepath has not been specified.");
		return 1;
	}

    app.InitManagers();

    QMainWindow *editorOrGameWindow = new QMainWindow();
    InitEditorOrGame(editorOrGameWindow, &app);
    ProjectManager::OpenProject(loadedProjectFilepath);

    app.MainLoop();

    return 0;
}
