#include "WindowMain.h"

#include "SingletonManager.h"

#include "Toolbar.h"
#include "WindowEventManager.h"

WindowMain *WindowMain::s_p_win = nullptr;

WindowMain::WindowMain() : Ui_WindowMain()
{

}

void WindowMain::InitFromMainBinary(QMainWindow *window, QApplication *application)
{
    WindowMain::s_p_win = new WindowMain();
    SingletonManager::GetInstance()->SetWindowMainSingleton(WindowMain::s_p_win);

    WindowMain::s_p_win->setupUi(window);

    WindowMain::s_p_win->p_mainWindow = window;
    WindowMain::s_p_win->p_mainWindow->showMaximized();

    WindowMain::s_p_win->p_app = application;

    Toolbar::Init();
}

WindowMain *WindowMain::GetInstance()
{
    // This will always be initialized,
    // since main inits it using WindowMain::InitFromMainBinary()
    return SingletonManager::GetInstance()->GetWindowSingleton();
}

QMainWindow *WindowMain::GetMainWindow() const
{
    return WindowMain::GetInstance()->p_mainWindow;
}

QApplication *WindowMain::GetApplication() const
{
    return WindowMain::GetInstance()->p_app;
}
