#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include "Bang/Vector2.h"

FORWARD   class Screen;

class Window
{
public:
    Window();
    virtual ~Window();

    void Render() const;
    void OnResize(int newWidth, int newHeight) const;

    int GetWidth() const;
    int GetHeight() const;
    Vector2 GetSize() const;

    virtual Screen *GetScreen() const;

protected:
    SDL_Window *m_sdlWindow = nullptr;
    SDL_GLContext m_sdlGLContext;
    Screen *m_screen = nullptr;
};

#endif // IWINDOW_H
