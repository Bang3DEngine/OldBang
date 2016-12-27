#include "EditorWindow.h"

#include "Debug.h"
#include "Toolbar.h"
#include "SingletonManager.h"
#include "WindowEventManager.h"

EditorWindow *EditorWindow::s_m_win = nullptr;

EditorWindow::EditorWindow() : Ui_EditorWindow()
{

}

void EditorWindow::InitFromMainBinary(QMainWindow *window, QApplication *application)
{
    IWindow::InitFromMainBinary(window, application);

    EditorWindow::s_m_win = new EditorWindow();
    SingletonManager::GetInstance()->SetEditorWindowSingleton(EditorWindow::s_m_win);

    EditorWindow::s_m_win->setupUi(window);

    EditorWindow::s_m_win->m_mainWindow = window;
    EditorWindow::s_m_win->m_mainWindow->showMaximized();

    EditorWindow::s_m_win->m_app = application;

    Toolbar::Init();
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
