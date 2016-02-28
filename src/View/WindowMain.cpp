#include "WindowMain.h"

WindowMain *WindowMain::win = nullptr;

WindowMain::WindowMain()  : Ui_WindowMain()
{
}

void WindowMain::Init(QMainWindow *window)
{
    if(win == nullptr)
    {
        win = new WindowMain();
        win->setupUi(window);
        window->showMaximized();
    }

    //Init inspector
    win->widgetInspector->addItem("ASDKDJASKDJAS");
    //
}

WindowMain *WindowMain::GetInstance()
{
    if(win == nullptr)
    {
        win = new WindowMain();
    }
    return win;
}
