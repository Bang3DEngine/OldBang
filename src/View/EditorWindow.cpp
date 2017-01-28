#include "EditorWindow.h"

#include "Debug.h"
#include "Toolbar.h"
#include "Explorer.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "EditorScene.h"
#include "SceneManager.h"
#include "SingletonManager.h"
#include "WindowEventManager.h"

EditorWindow *EditorWindow::s_win = nullptr;

EditorWindow::EditorWindow(QWidget *parent) :
    QObject(parent), Ui_EditorWindow()
{

}

void EditorWindow::InitFromMainBinary(QMainWindow *window, QApplication *application)
{
    IWindow::InitFromMainBinary(window, application);

    EditorWindow::s_win = new EditorWindow();
    SingletonManager::GetInstance()->SetWindowSingleton(EditorWindow::s_win);

    EditorWindow::s_win->setupUi(window);

    EditorWindow::s_win->m_mainWindow = window;
    EditorWindow::s_win->m_mainWindow->showMaximized();

    EditorWindow::s_win->m_app = application;

    Toolbar::Init();

    Hierarchy::GetInstance()->OnWindowShown();
    Explorer::GetInstance()->OnWindowShown();
    Inspector::GetInstance()->OnWindowShown();

    QObject::connect(EditorWindow::s_win->tabContainerSceneGame, SIGNAL(currentChanged(int)),
                     EditorWindow::s_win, SLOT(OnTabSceneGameChanged(int)));

    QDockWidget *hierarchyDock = EditorWindow::s_win->dockHierarchy;
    QDockWidget *inspectorDock = EditorWindow::s_win->dockInspector;
    QDockWidget *explorerDock = EditorWindow::s_win->dockExplorer;
    QDockWidget *loggerDock = EditorWindow::s_win->dockLogger;
    window->tabifyDockWidget(explorerDock, loggerDock);
    explorerDock->raise();

    window->setCorner(Qt::TopLeftCorner,     Qt::LeftDockWidgetArea);
    //window->setCorner(Qt::BottomLeftCorner,  Qt::LeftDockWidgetArea);
    window->setCorner(Qt::BottomLeftCorner,  Qt::BottomDockWidgetArea);
    window->setCorner(Qt::TopRightCorner,    Qt::RightDockWidgetArea);
    window->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
}

EditorWindow *EditorWindow::GetInstance()
{
    // This will always be initialized,
    // since main inits it using EditorWindow::InitFromMainBinary()
    return SingletonManager::GetInstance() ?
                static_cast<EditorWindow*>(SingletonManager::GetInstance()->GetWindowSingleton()) :
                nullptr;
}

QMainWindow *EditorWindow::GetMainWindow() const
{
    return EditorWindow::GetInstance()->m_mainWindow;
}

QApplication *EditorWindow::GetApplication() const
{
    return EditorWindow::GetInstance()->m_app;
}

void EditorWindow::OnTabSceneGameChanged(int index)
{
    EditorScene *edScene = static_cast<EditorScene*>(SceneManager::GetActiveScene());
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
}
