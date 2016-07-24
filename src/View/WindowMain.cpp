#include "WindowMain.h"

#include "Toolbar.h"
#include "SingletonManager.h"
#include "WindowEventManager.h"

WindowMain *WindowMain::s_m_win = nullptr;

WindowMain::WindowMain() : Ui_WindowMain()
{

}

void WindowMain::InitFromMainBinary(QMainWindow *window, QApplication *application)
{
    IWindow::InitFromMainBinary(window, application);

    WindowMain::s_m_win = new WindowMain();
    SingletonManager::GetInstance()->SetWindowMainSingleton(WindowMain::s_m_win);

    WindowMain::s_m_win->setupUi(window);

    WindowMain::s_m_win->m_mainWindow = window;
    WindowMain::s_m_win->m_mainWindow->showMaximized();

    WindowMain::s_m_win->m_app = application;

    Toolbar::Init();
}

WindowMain *WindowMain::GetInstance()
{
    // This will always be initialized,
    // since main inits it using WindowMain::InitFromMainBinary()
    return static_cast<WindowMain*>(SingletonManager::GetInstance()->GetWindowSingleton());
}

QMainWindow *WindowMain::GetMainWindow() const
{
    return WindowMain::GetInstance()->m_mainWindow;
}

QApplication *WindowMain::GetApplication() const
{
    return WindowMain::GetInstance()->m_app;
}
