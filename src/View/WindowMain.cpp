#include "WindowMain.h"

#include <QApplication>

WindowMain *WindowMain::win = nullptr;
QMainWindow *WindowMain::mainWindow = nullptr;
QApplication *WindowMain::app = nullptr;

WindowMain::WindowMain()  : Ui_WindowMain()
{
}

void WindowMain::Init(QMainWindow *window, QApplication *application)
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

QApplication *WindowMain::GetApplication() const
{
    return app;
}
