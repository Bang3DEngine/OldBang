#include "Array.h"

#include <iostream>

#include <QTextStream>

#include "glm/glm.hpp"

#include "Debug.h"

#include "Input.h"
#include "Timer.h"
#include "Debug.h"
#include "Camera.h"
#include "Screen.h"
#include "Shader.h"
#include "XMLNode.h"
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
    // Load dark Window theme
    QFile f((Persistence::c_EngineAssetsRootAbsolute + "/qdarkstyle/style.qss").ToQString()
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

    // Set Font
    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPointSize(8);
    app->setFont(font);
}

void InitEditor(QMainWindow *window, Application *app)
{
    SingletonManager::InitSingletonManagerFromMainBinary();
    #ifdef BANG_EDITOR
    ShortcutManager::InitFromMainBinary(); // It must go before Application init
    #endif

    #ifdef BANG_EDITOR
    EditorWindow::InitFromMainBinary(window, app);
    #else
    GameWindow::InitFromMainBinary(window, app);
    #endif

    Screen::InitFromMainBinary();
    Time::InitFromMainBinary();
    Input::InitFromMainBinary();
}

int main(int argc, char *argv[])
{
    // Init engine paths, by looking at executable location:
    String engineDir = argv[0];
    String binDir = Persistence::GetDir(engineDir);
    String engineRootDirAbs = Persistence::GetDirUp(binDir);
    Persistence::c_EngineRootAbsolute = engineRootDirAbs;
    Persistence::c_EngineAssetsRootAbsolute =
            Persistence::c_EngineRootAbsolute + "/EngineAssets";
    //

    #ifdef BANG_EDITOR
    // Select project
    QApplication *selectProjectApp = new QApplication(argc, argv);
    QMainWindow *selectProjectWindow = new QMainWindow();
    LoadStylesheet(selectProjectApp);
    String loadedProjectFilepath =
            SelectProjectWindow::ExecAndGetProjectFilepath(selectProjectWindow,
                                                           selectProjectApp);
    //

    if (loadedProjectFilepath.Empty())
    {
        return 0; // If the select project window is closed by the user
    }
    #endif

    // Editor / Game
    Application app(argc, argv);
    QMainWindow *editorOrGameWindow = new QMainWindow();
    #ifdef BANG_EDITOR
    LoadStylesheet(&app);
    #endif
    InitEditor(editorOrGameWindow, &app);
    ProjectManager::OpenProject(loadedProjectFilepath); // Load previously selected project
    return app.exec();
    //
}
