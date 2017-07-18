#include "Bang/Window.h"

#include "Bang/Debug.h"
#include "Bang/Screen.h"

Window::Window()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        Debug_Error("Failed to init SDL");
        return;
    }

    m_sdlWindow = SDL_CreateWindow("Bang",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   512, 512,
                                   SDL_WINDOW_RESIZABLE |
                                   SDL_WINDOW_OPENGL);

    m_sdlGLContext = SDL_GL_CreateContext(m_sdlWindow);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_screen = new Screen();
    m_screen->Initialize();

    OnResize(GetWidth(), GetHeight());
}

Window::~Window()
{
    delete m_screen;
    SDL_DestroyWindow(m_sdlWindow);
}

void Window::Render() const
{
    m_screen->Render();
    SDL_GL_SwapWindow(m_sdlWindow);
}

int Window::GetWidth() const { return GetSize().x; }
int Window::GetHeight() const { return GetSize().y; }
Vector2 Window::GetSize() const
{
    int width, height;
    SDL_GetWindowSize(m_sdlWindow, &width, &height);
    return Vector2(width, height);
}

void Window::OnResize(int newWidth, int newHeight) const
{
    m_screen->OnResize(newWidth, newHeight);
}

Screen *Window::GetScreen() const
{
    return m_screen;
}

