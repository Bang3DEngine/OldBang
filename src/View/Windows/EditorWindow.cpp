#include "EditorWindow.h"

#include "Debug.h"
#include "Toolbar.h"
#include "Explorer.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "EditorState.h"
#include "EditorScene.h"
#include "SceneManager.h"
#include "SingletonManager.h"
#include "WindowEventManager.h"
#include "EditorPlayStopFlowController.h"

EditorWindow *EditorWindow::s_win = nullptr;

EditorWindow::EditorWindow(QWidget *parent) :
    QObject(parent), Ui_EditorWindow()
{

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
    QDockWidget *explorerDock = EditorWindow::s_win->dockExplorer;
    QDockWidget *ConsoleDock = EditorWindow::s_win->dockConsole;

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
    ConsoleDock->setWindowFlags(Qt::WindowTitleHint);

    window->tabifyDockWidget(explorerDock, ConsoleDock);
    explorerDock->raise();

    window->setCorner(Qt::TopLeftCorner,     Qt::LeftDockWidgetArea);
    //window->setCorner(Qt::BottomLeftCorner,  Qt::LeftDockWidgetArea);
    window->setCorner(Qt::BottomLeftCorner,  Qt::BottomDockWidgetArea);
    window->setCorner(Qt::TopRightCorner,    Qt::RightDockWidgetArea);
    window->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
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
