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
    uint newFlags = flags;
    newFlags &= ~SDL_WINDOW_RESIZABLE;
    Window::Create(newFlags);
}

void DialogWindow::CenterInParent()
{
    if (GetParentWindow())
    {
        Vector2i centeredPos = GetParentWindow()->GetPosition() +
                               GetParentWindow()->GetSize() / 2 -
                               GetSize() / 2;
        SetPosition(centeredPos.x, centeredPos.y);
    }
}
