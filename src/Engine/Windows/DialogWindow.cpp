#include "Bang/DialogWindow.h"

#include <SDL2/SDL.h>
#include "Bang/Input.h"

USING_NAMESPACE_BANG

DialogWindow::DialogWindow(Window *parentWindow)
{
    SetParent(parentWindow);
}

DialogWindow::~DialogWindow()
{

}

void DialogWindow::Create(uint flags)
{
    Window::Create(flags & ~SDL_WINDOW_RESIZABLE);
}
