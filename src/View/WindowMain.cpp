#include "WindowMain.h"

#include <QApplication>

WindowMain *WindowMain::win = nullptr;
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
        window->showMaximized();
        app = application;
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

QApplication *WindowMain::GetApplication() const
{
    return app;
}
