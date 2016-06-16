#include "WindowMain.h"

#include "SingletonManager.h"
#include "WindowEventManager.h"

WindowMain *WindowMain::win = nullptr;

void WindowMain::InitFromMainBinary(QMainWindow *window, QApplication *application)
{
    win = new WindowMain();
    SingletonManager::GetInstance()->SetWindowMainSingleton(win);

    win->setupUi(window);

    win->mainWindow = window;
    win->mainWindow->showMaximized();

    win->app = application;

    win->windowEventManager->connect(
                     win->buttonGlobalCoords, SIGNAL(clicked()),
                     win->windowEventManager,
                     SLOT(NotifyButtonGlobalCoordsClicked()));
}

WindowMain *WindowMain::GetInstance()
{
    // This will always be initialized,
    // since main inits it using WindowMain::InitFromMainBinary()
    return SingletonManager::GetInstance()->GetWindowMainSingleton();
}

QMainWindow *WindowMain::GetMainWindow()
{
    return GetInstance()->mainWindow;
}

QApplication *WindowMain::GetApplication() const
{
    return GetInstance()->app;
}
