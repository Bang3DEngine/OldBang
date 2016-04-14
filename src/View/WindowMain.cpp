#include "WindowMain.h"

WindowMain *WindowMain::win = nullptr;
QMainWindow *WindowMain::mainWindow = nullptr;

WindowMain::WindowMain()  : Ui_WindowMain()
{
}

void WindowMain::Init(QMainWindow *window)
{
    if(win == nullptr)
    {
        win = new WindowMain();
        win->setupUi(window);

        mainWindow = window;
        mainWindow->showMaximized();
    }
}

WindowMain *WindowMain::GetInstance()
{
    if(win == nullptr)
    {
        win = new WindowMain();
    }
    return win;
}

QMainWindow *WindowMain::GetMainWindow()
{
    return mainWindow;
}
