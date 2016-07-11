#include "WindowMain.h"

#include "SingletonManager.h"
#include "WindowEventManager.h"

#include "Toolbar.h"

WindowMain *WindowMain::s_p_win = nullptr;

void WindowMain::InitFromMainBinary(QMainWindow *window, QApplication *application)
{
    s_p_win = new WindowMain();
    SingletonManager::GetInstance()->SetWindowMainSingleton(s_p_win);

    s_p_win->setupUi(window);

    s_p_win->p_mainWindow = window;
    s_p_win->p_mainWindow->showMaximized();

    s_p_win->p_app = application;

    Toolbar::Init();
}

WindowMain *WindowMain::GetInstance()
{
    // This will always be initialized,
    // since main inits it using WindowMain::InitFromMainBinary()
    return SingletonManager::GetInstance()->GetWindowMainSingleton();
}

QMainWindow *WindowMain::GetMainWindow()
{
    return GetInstance()->p_mainWindow;
}

QApplication *WindowMain::GetApplication() const
{
    return GetInstance()->p_app;
}
