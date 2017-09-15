#include "Bang/Window.h"

#include "Bang/GL.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Texture2D.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"

USING_NAMESPACE_BANG

Window::Window()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        Debug_Error("Failed to init SDL");
        return;
    }

    Vector2i winSize(512);
    m_sdlWindow = SDL_CreateWindow("Bang",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   winSize.x, winSize.y,
                                   SDL_WINDOW_RESIZABLE  |
                                   SDL_WINDOW_OPENGL);

    m_sdlGLContext = SDL_GL_CreateContext(m_sdlWindow);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    Initialize();

    m_screenRenderTexture = new Texture2D();
}

Window::~Window()
{
    SDL_DestroyWindow(m_sdlWindow);
}

void Window::Initialize()
{
    glewExperimental = true;

    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        Debug_Error("Glew init error: " << glewGetErrorString(glewError));
    }

    GL::Enable(GL::Test::Depth);
    GL::Enable(GL::Test::Stencil);
    GL::Enable(GL::Test::CullFace);
}

void Window::SwapBuffers() const
{
    SDL_GL_SwapWindow(m_sdlWindow);
}

void Window::Resize(int w, int h)
{
    SDL_SetWindowSize(m_sdlWindow, w, h);
}

void Window::OnResize(int newWidth, int newHeight)
{
    m_screenRenderTexture->Resize(newWidth, newHeight);
}

int Window::GetWidth() const { return GetSize().x; }
int Window::GetHeight() const { return GetSize().y; }
Vector2i Window::GetSize() const
{
    int width, height;
    SDL_GetWindowSize(m_sdlWindow, &width, &height);
    return Vector2i(width, height);
}

Texture2D *Window::GetScreenRenderTexture() const
{
    return m_screenRenderTexture;
}

int Window::GetHeightS()
{
    return Window::GetInstance()->GetHeight();
}

int Window::GetWidthS()
{
    return Window::GetInstance()->GetWidth();
}

Window *Window::GetInstance()
{
    return Application::GetInstance()->GetWindow();
}
