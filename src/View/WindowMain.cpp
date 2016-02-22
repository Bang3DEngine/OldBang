#include "WindowMain.h"

WindowMain *WindowMain::win = nullptr;

WindowMain::WindowMain()  : Ui_WindowMain()
{
}

WindowMain *WindowMain::GetInstance()
{
    if(win == nullptr)
    {
        win = new WindowMain();
    }
    return win;
}
