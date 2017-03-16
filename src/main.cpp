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
#include "IO.h"
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
    QFile f((IO::GetEngineAssetsRootAbs() + "/qdarkstyle/style.qss").ToQString()
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
    Time::InitFromMainBinary();
    Input::InitFromMainBinary();
    IO::InitFromMainBinary();
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

using namespace std;
#include <QProcess>
#include "SystemUtils.h"
int main(int argc, char **argv)
{
    String out;
    bool success;
    SystemUtils::System("/usr/bin/g++ -c  /home/sephirot47/Arcanoid/Assets/Behaviours/Block.cpp    -O0 -g  -DBANG_EDITOR  -Wl,--export-dynamic --std=c++11  -I. -I/home/sephirot47/Arcanoid -I/home/sephirot47/Arcanoid/Assets -I/home/sephirot47/Arcanoid/Assets/Behaviours -I/home/sephirot47/Arcanoid/Assets/Materials -I/home/sephirot47/Arcanoid/Assets/Materials/Textures -I/home/sephirot47/Arcanoid/Assets/Prefabs -I/home/sephirot47/Arcanoid/Assets/Scenes -I/home/sephirot47/Arcanoid/GameData -I/home/sephirot47/Arcanoid/GameData/Assets -I/home/sephirot47/Arcanoid/GameData/Assets/Behaviours -I/home/sephirot47/Arcanoid/GameData/Assets/Materials -I/home/sephirot47/Arcanoid/GameData/Assets/Materials/Textures -I/home/sephirot47/Arcanoid/GameData/Assets/Prefabs -I/home/sephirot47/Arcanoid/GameData/Assets/Scenes -I/home/sephirot47/Arcanoid/GameData/EngineAssets -I/home/sephirot47/Arcanoid/GameData/EngineAssets/Fonts -I/home/sephirot47/Arcanoid/GameData/EngineAssets/Icons -I/home/sephirot47/Arcanoid/GameData/EngineAssets/Materials -I/home/sephirot47/Arcanoid/GameData/EngineAssets/Materials/UI -I/home/sephirot47/Arcanoid/GameData/EngineAssets/Meshes -I/home/sephirot47/Arcanoid/GameData/EngineAssets/qdarkstyle -I/home/sephirot47/Arcanoid/GameData/EngineAssets/qdarkstyle/rc -I/home/sephirot47/Arcanoid/GameData/EngineAssets/Shaders -I/home/sephirot47/Arcanoid/GameData/EngineAssets/Shaders/Include -I/home/sephirot47/Arcanoid/GameData/EngineAssets/Shaders/UI -I/home/sephirot47/Arcanoid/GameData/EngineAssets/Textures -I/home/sephirot47/Arcanoid/GameData/Libraries -I/home/sephirot47/Arcanoid/Project -I//home/sephirot47/Bang -I//home/sephirot47/Bang/bin -I//home/sephirot47/Bang/bin/objEditor -I//home/sephirot47/Bang/bin/objGame -I//home/sephirot47/Bang/config -I//home/sephirot47/Bang/EngineAssets -I//home/sephirot47/Bang/EngineAssets/Fonts -I//home/sephirot47/Bang/EngineAssets/Icons -I//home/sephirot47/Bang/EngineAssets/Materials -I//home/sephirot47/Bang/EngineAssets/Materials/UI -I//home/sephirot47/Bang/EngineAssets/Meshes -I//home/sephirot47/Bang/EngineAssets/qdarkstyle -I//home/sephirot47/Bang/EngineAssets/qdarkstyle/rc -I//home/sephirot47/Bang/EngineAssets/Shaders -I//home/sephirot47/Bang/EngineAssets/Shaders/Include -I//home/sephirot47/Bang/EngineAssets/Shaders/UI -I//home/sephirot47/Bang/EngineAssets/Textures -I//home/sephirot47/Bang/scripts -I//home/sephirot47/Bang/src -I//home/sephirot47/Bang/src/Domain -I//home/sephirot47/Bang/src/Domain/Engine -I//home/sephirot47/Bang/src/Domain/Engine/Assets -I//home/sephirot47/Bang/src/Domain/Engine/Assets/Meshes -I//home/sephirot47/Bang/src/Domain/Engine/Audio -I//home/sephirot47/Bang/src/Domain/Engine/Components -I//home/sephirot47/Bang/src/Domain/Engine/DataStructures -I//home/sephirot47/Bang/src/Domain/Engine/Debug -I//home/sephirot47/Bang/src/Domain/Engine/GameObjects -I//home/sephirot47/Bang/src/Domain/Engine/GraphicPipeline -I//home/sephirot47/Bang/src/Domain/Engine/Interfaces -I//home/sephirot47/Bang/src/Domain/Engine/Others -I//home/sephirot47/Bang/src/Domain/Engine/Physics -I//home/sephirot47/Bang/src/Domain/Engine/Physics/glm -I//home/sephirot47/Bang/src/Domain/Engine/Physics/glm/detail -I//home/sephirot47/Bang/src/Domain/Engine/Physics/glm/gtc -I//home/sephirot47/Bang/src/Domain/Engine/Physics/glm/gtx -I//home/sephirot47/Bang/src/Domain/Graphics -I//home/sephirot47/Bang/src/Domain/Graphics/Buffers -I//home/sephirot47/Bang/src/Domain/Graphics/Interfaces -I//home/sephirot47/Bang/src/Domain/Graphics/Shaders -I//home/sephirot47/Bang/src/Domain/Interfaces -I//home/sephirot47/Bang/src/Persistence -I//home/sephirot47/Bang/src/Persistence/Behaviours -I//home/sephirot47/Bang/src/Persistence/Files -I//home/sephirot47/Bang/src/Persistence/IO -I//home/sephirot47/Bang/src/Persistence/Project -I//home/sephirot47/Bang/src/Persistence/XML -I//home/sephirot47/Bang/src/Qt_MOCs -I//home/sephirot47/Bang/src/View -I//home/sephirot47/Bang/src/View/Console -I//home/sephirot47/Bang/src/View/Dialogs -I//home/sephirot47/Bang/src/View/DragDrop -I//home/sephirot47/Bang/src/View/EditorCamera -I//home/sephirot47/Bang/src/View/EditorGameObject -I//home/sephirot47/Bang/src/View/EditorGameObject/EditorRectTransform -I//home/sephirot47/Bang/src/View/EditorScene -I//home/sephirot47/Bang/src/View/Explorer -I//home/sephirot47/Bang/src/View/Hierarchy -I//home/sephirot47/Bang/src/View/Inspector -I//home/sephirot47/Bang/src/View/Inspector/AttributeWidgets -I//home/sephirot47/Bang/src/View/Inspector/Inspectables -I//home/sephirot47/Bang/src/View/Interfaces -I//home/sephirot47/Bang/src/View/Windows -I//home/sephirot47/Bang/tmp   -lGLEW -lGL -lpthread -L/usr/lib/x86_64-linux-gnu  -fPIC -o /home/sephirot47/Arcanoid/Libraries/Block.o",
                        {}, &out, &success);
    /*
    QProcess process;
    process.setStandardOutputFile("/home/sephirot47/Bang/test.txt");
    process.start("echo wololo");
    bool ok = process.waitForFinished(999999);
    */

    Application app(argc, argv);
    LoadStylesheet(&app);

    InitSingletonManager();

    String executableDir = Application::applicationDirPath();

    // Init engine paths, by looking at executable location:
    #ifdef BANG_EDITOR
    String engineRootDirAbs = IO::GetDirUp(executableDir);
    IO::GetInstance()->c_EngineRootAbsolute = engineRootDirAbs;
    IO::GetInstance()->c_EngineAssetsRootAbsolute =
            IO::GetEngineRootAbs() + "/EngineAssets";
    #else
    IO::GetInstance()->c_EngineRootAbsolute = executableDir + "/GameData";
    IO::GetInstance()->c_EngineAssetsRootAbsolute =
            IO::GetEngineRootAbs() + "/EngineAssets";
    #endif

    String loadedProjectFilepath = "";
    #ifdef BANG_EDITOR
    // Select project
    QMainWindow *selectProjectWindow = new QMainWindow();
    loadedProjectFilepath =
            SelectProjectWindow::ExecAndGetProjectFilepath(selectProjectWindow,
                                                           &app);
    #else
    loadedProjectFilepath = executableDir + "/GameData/Game.bproject";
    #endif

    ASSERT (!loadedProjectFilepath.Empty(),
            "The project filepath has not been specified.", return 0);

    app.InitManagers();

    QMainWindow *editorOrGameWindow = new QMainWindow();
    InitEditorOrGame(editorOrGameWindow, &app);
    ProjectManager::OpenProject(loadedProjectFilepath); // Load selected project

    app.MainLoop();
    return 0;
}
