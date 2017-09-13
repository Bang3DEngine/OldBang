#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include "Bang/Vector2.h"

NAMESPACE_BANG_BEGIN

FORWARD class GraphicPipeline;

class Window
{
public:
    Window();
    virtual ~Window();


    void Initialize();
    void Render() const;
    void OnResize(int newWidth, int newHeight);

    void Resize(int w, int h);

    int GetWidth() const;
    int GetHeight() const;
    Vector2i GetSize() const;

    static float GetAspectRatioS();
    static int GetHeightS();
    static int GetWidthS();

    static Window *GetInstance();
    GraphicPipeline *GetGraphicPipeline() const;

protected:
    SDL_Window *m_sdlWindow = nullptr;
    SDL_GLContext m_sdlGLContext;

    GraphicPipeline *m_gPipeline = nullptr;
};

NAMESPACE_BANG_END

#endif // IWINDOW_H
