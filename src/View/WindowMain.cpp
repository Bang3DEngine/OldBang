#include "WindowMain.h"

#include <QApplication>

WindowMain *WindowMain::win = nullptr;

void WindowMain::Init(QMainWindow *window, QApplication *application)
{
    Logger_Log("WindowMain Init");

    win = new WindowMain();
    win->setupUi(window);

    win->mainWindow = window;
    win->mainWindow->showMaximized();

    win->app = application;
}

void WindowMain::Init(WindowMain *singletonWindowMain)
{
    win = singletonWindowMain;
}

WindowMain *WindowMain::GetInstance()
{
    //This will always be initialized, since main inits it using WindowMain::Init()
   // Logger_Log("GetInstance: " << win);
    return win;
}

QMainWindow *WindowMain::GetMainWindow()
{
    return GetInstance()->mainWindow;
}

QApplication *WindowMain::GetApplication() const
{
    return GetInstance()->app;
}
