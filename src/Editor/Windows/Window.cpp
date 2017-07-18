#include "Bang/Window.h"

#include "Bang/Debug.h"
#include "Bang/Screen.h"
#include "Bang/SingletonManager.h"

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
                                   512, 512, SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_sdlGLContext = SDL_GL_CreateContext(m_sdlWindow);

    m_screen = new Screen();
}

Window::~Window()
{
    delete m_screen;
    delete m_sdlWindow;
}

Screen *Window::GetScreen() const
{
    return m_screen;
}

