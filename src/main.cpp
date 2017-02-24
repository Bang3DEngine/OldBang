#include "Array.h"

#include <iostream>

#include <QTextStream>
#include <QStyleFactory>

#include "glm/glm.hpp"

#include "Debug.h"
#include "Input.h"
#include "Timer.h"
#include "Debug.h"
#include "Camera.h"
#include "Shader.h"
#include "Screen.h"
#include "XMLNode.h"
#include "AudioClip.h"
#include "Behaviour.h"
#include "Texture2D.h"
#include "XMLParser.h"
#include "GameObject.h"
#include "FileReader.h"
#include "Persistence.h"
#include "Application.h"
#include "Framebuffer.h"
#include "SceneManager.h"
#include "MeshRenderer.h"
#include "ShaderProgram.h"
#include "ProjectManager.h"
#include "SingletonManager.h"

#ifdef BANG_EDITOR

#include "EditorScene.h"
#include "EditorWindow.h"
#include "ShortcutManager.h"
#include "SelectProjectWindow.h"

#else

#include "GameWindow.h"

#endif

#include "Array.h"
#include "String.h"

void LoadStylesheet(QApplication *app)
{
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

    /*
    // Load dark Window theme
    QFile f((Persistence::GetEngineAssetsRootAbs() + "/qdarkstyle/style.qss").ToQString()
            );
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
    */
    // Set Font
    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPointSize(8);
    app->setFont(font);
}

void InitSingletonManager()
{
    SingletonManager::InitSingletonManagerFromMainBinary();
    #ifdef BANG_EDITOR
    ShortcutManager::InitFromMainBinary(); // It must go before Application init
    #endif

    Time::InitFromMainBinary();
    Input::InitFromMainBinary();
    Persistence::InitFromMainBinary();
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

int main(int argc, char **argv)
{
    // Get the executable dir
    QApplication *preApp = new QApplication(argc, argv);
    String executableDir = preApp->applicationDirPath();
    preApp->exit(0);
    delete preApp;

    InitSingletonManager();

    // Init engine paths, by looking at executable location:
    #ifdef BANG_EDITOR
    String engineRootDirAbs = Persistence::GetDirUp(executableDir);
    Persistence::GetInstance()->c_EngineRootAbsolute = engineRootDirAbs;
    Persistence::GetInstance()->c_EngineAssetsRootAbsolute =
            Persistence::GetEngineRootAbs() + "/EngineAssets";
    #else
    Persistence::GetInstance()->c_EngineRootAbsolute = executableDir + "/GameData";
    Persistence::GetInstance()->c_EngineAssetsRootAbsolute =
            Persistence::GetEngineRootAbs() + "/EngineAssets";
    #endif



    String loadedProjectFilepath = "";

    #ifdef BANG_EDITOR
    // Select project
    QApplication *selectProjectApp = new QApplication(argc, argv);
    QMainWindow *selectProjectWindow = new QMainWindow();
    LoadStylesheet(selectProjectApp);
    loadedProjectFilepath =
            SelectProjectWindow::ExecAndGetProjectFilepath(selectProjectWindow,
                                                           selectProjectApp);
    delete selectProjectApp;
    #else
    loadedProjectFilepath = executableDir + "/GameData/Game.bproject";
    #endif
    //

    ASSERT (!loadedProjectFilepath.Empty(),
            "The project filepath has not been specified.", return 0);

    // Editor / Game
    Application app(argc, argv);
    QMainWindow *editorOrGameWindow = new QMainWindow();
    #ifdef BANG_EDITOR
    LoadStylesheet(&app);
    #endif
    InitEditorOrGame(editorOrGameWindow, &app);
    ProjectManager::OpenProject(loadedProjectFilepath); // Load selected project
    return app.exec();
    //
}
