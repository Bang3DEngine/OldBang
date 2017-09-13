#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include "Bang/Vector2.h"

NAMESPACE_BANG_BEGIN

FORWARD class Texture2D;

class Window
{
public:
    Window();
    virtual ~Window();

    void Initialize();
    void SwapBuffers() const;

    void Resize(int newWidth, int newHeight);
    void OnResize(int newWidth, int newHeight);

    int GetWidth() const;
    int GetHeight() const;
    Vector2i GetSize() const;
    Texture2D *GetScreenRenderTexture() const;

    static float GetAspectRatioS();
    static int GetHeightS();
    static int GetWidthS();

    static Window *GetInstance();

protected:
    SDL_Window *m_sdlWindow = nullptr;
    SDL_GLContext m_sdlGLContext;

    Texture2D *m_screenRenderTexture = nullptr;
};

NAMESPACE_BANG_END

#endif // IWINDOW_H
