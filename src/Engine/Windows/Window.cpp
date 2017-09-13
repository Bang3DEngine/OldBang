#include "Bang/Window.h"

#include "Bang/GL.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/GraphicPipeline.h"

USING_NAMESPACE_BANG

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

    SDL_SetWindowBrightness(m_sdlWindow, 0.5f);

    Initialize();
    OnResize(GetWidth(), GetHeight());
}

Window::~Window()
{
    SDL_DestroyWindow(m_sdlWindow);
    if (m_gPipeline) { delete m_gPipeline; }
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

    Vector2i windowSize = GetSize();
    m_gPipeline = new GraphicPipeline(windowSize.x, windowSize.y);
}

void Window::Render() const
{
    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        m_gPipeline->RenderScene(activeScene);
    }

    SDL_GL_SwapWindow(m_sdlWindow);
}

void Window::Resize(int w, int h)
{
    SDL_SetWindowSize(m_sdlWindow, w, h);
}

void Window::OnResize(int w, int h)
{
    ENSURE(m_gPipeline);

    GL::SetViewport(0, 0, w, h);

    m_gPipeline->OnResize(w, h);
    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        activeScene->_OnResize(w,h);
    }
}

int Window::GetWidth() const { return GetSize().x; }
int Window::GetHeight() const { return GetSize().y; }
Vector2i Window::GetSize() const
{
    int width, height;
    SDL_GetWindowSize(m_sdlWindow, &width, &height);
    return Vector2i(width, height);
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

GraphicPipeline *Window::GetGraphicPipeline() const
{
    return m_gPipeline;
}

