#include "Bang/EditorWindow.h"

#include "Bang/Debug.h"
#include "Bang/Project.h"
#include "Bang/Toolbar.h"
#include "Bang/Explorer.h"
#include "Bang/Hierarchy.h"
#include "Bang/Inspector.h"
#include "Bang/EditorState.h"
#include "Bang/EditorScene.h"
#include "Bang/SceneManager.h"
#include "Bang/ProjectManager.h"
#include "Bang/SingletonManager.h"
#include "Bang/WindowEventManager.h"
#include "Bang/EditorPlayStopFlowController.h"

EditorWindow *EditorWindow::s_win = nullptr;

EditorWindow::EditorWindow(QWidget *parent) :
    QObject(parent), Ui_EditorWindow()
{
    m_refreshTimer.start(1000);
    QObject::connect(&m_refreshTimer, SIGNAL(timeout()),
                     this, SLOT(Refresh()));
}

void EditorWindow::InitFromMainBinary(QMainWindow *window, QApplication *application)
{
    EditorWindow::s_win = new EditorWindow();
    EditorWindow::s_win->m_editorState        = new EditorState();
    EditorWindow::s_win->m_winEventManager    = new WindowEventManager();
    EditorWindow::s_win->m_playStopController = new EditorPlayStopFlowController();

    SingletonManager::Set<IWindow>(EditorWindow::s_win);

    EditorWindow::s_win->m_mainWindow = window;
    EditorWindow::s_win->m_app = application;

    EditorWindow::s_win->setupUi(window);
    EditorWindow::s_win->m_mainWindow->showMaximized();

    Toolbar::Init();

    Hierarchy::GetInstance()->OnWindowShown();
    Explorer::GetInstance()->OnWindowShown();
    Inspector::GetInstance()->OnWindowShown();
    Console::GetInstance()->OnWindowShown();

    QObject::connect(EditorWindow::s_win->tabContainerSceneGame,
                     SIGNAL(currentChanged(int)),
                     EditorWindow::s_win, SLOT(OnTabSceneGameChanged(int)));

    QDockWidget *hierarchyDock = EditorWindow::s_win->dockHierarchy;
    QDockWidget *inspectorDock = EditorWindow::s_win->dockInspector;
    QDockWidget *explorerDock  = EditorWindow::s_win->dockExplorer;
    QDockWidget *consoleDock   = EditorWindow::s_win->dockConsole;

    // Take out dock titles above
    /*
    hierarchyDock->setTitleBarWidget(new QWidget());
    inspectorDock->setTitleBarWidget(new QWidget());
    explorerDock->setTitleBarWidget(new QWidget());
    ConsoleDock->setTitleBarWidget(new QWidget());
    */

    // Take out 'X' buttons
    hierarchyDock->setWindowFlags(Qt::WindowTitleHint);
    inspectorDock->setWindowFlags(Qt::WindowTitleHint);
    explorerDock->setWindowFlags(Qt::WindowTitleHint);
    consoleDock->setWindowFlags(Qt::WindowTitleHint);

    window->tabifyDockWidget(explorerDock, consoleDock);
    explorerDock->raise();

    window->setCorner(Qt::TopLeftCorner,     Qt::LeftDockWidgetArea);
    //window->setCorner(Qt::BottomLeftCorner,  Qt::LeftDockWidgetArea);
    window->setCorner(Qt::BottomLeftCorner,  Qt::BottomDockWidgetArea);
    window->setCorner(Qt::TopRightCorner,    Qt::RightDockWidgetArea);
    window->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
}

bool EditorWindow::IsSceneTabActive() const
{
    return tabContainerSceneGame->currentWidget() == tabScene;
}

bool EditorWindow::IsGameTabActive() const
{
    return tabContainerSceneGame->currentWidget() == tabGame;
}

void EditorWindow::RefreshDocksAndWindowTitles()
{
    String windowTitle = "Bang - ";
    windowTitle += ProjectManager::GetCurrentProject()->GetProjectName();

    String sceneTitle = SceneManager::GetOpenSceneFilepath();
    sceneTitle = IO::GetFileName(sceneTitle);
    if (!SceneManager::IsCurrentSceneSaved())
    {
        sceneTitle += " *";
    }
    windowTitle += " - " + sceneTitle;

    tabContainerSceneGame->setTabText(0, sceneTitle.ToQString());
    m_mainWindow->setWindowTitle(windowTitle.ToQString());
}

EditorWindow *EditorWindow::GetInstance()
{
    IWindow *w = SingletonManager::Get<IWindow>();
    return w ? Object::SCast<EditorWindow>(w) : nullptr;
}

QMainWindow *EditorWindow::GetMainWindow() const
{
    EditorWindow *ew = EditorWindow::GetInstance();
    return ew ? ew->m_mainWindow : nullptr;
}

QApplication *EditorWindow::GetApplication() const
{
    EditorWindow *ew = EditorWindow::GetInstance();
    return ew ? ew->m_app : nullptr;
}

void EditorWindow::OnTabSceneGameChanged(int index)
{
    EditorScene *edScene =
            Object::SCast<EditorScene>(SceneManager::GetActiveScene());
	if (!edScene) { return; }

    bool scene = tabContainerSceneGame->widget(index) == tabScene;
    if (scene)
    {
        tabGameVLayout->takeAt(0);
        tabGameVLayout->takeAt(1);
        tabSceneVLayout->addWidget(screenToolbar, 0);
        tabSceneVLayout->addWidget(screen, 9999);

        edScene->SetEditorCamera();
    }
    else // Game
    {
        tabSceneVLayout->takeAt(0);
        tabSceneVLayout->takeAt(1);
        tabGameVLayout->addWidget(screenToolbar, 0);
        tabGameVLayout->addWidget(screen, 9999);

        edScene->SetFirstFoundCameraOrDefaultOne();
    }

    Toolbar::GetInstance()->OnSceneGameTabChanged();
}

void EditorWindow::Refresh()
{
    RefreshDocksAndWindowTitles();
}
