#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include "Bang/WinUndef.h"

class Screen;
class Window
{
public:
    Window();
    virtual ~Window();

    virtual Screen *GetScreen() const;

protected:
    SDL_Window *m_sdlWindow = nullptr;
    SDL_GLContext m_sdlGLContext;
    Screen *m_screen = nullptr;
};

#endif // IWINDOW_H
