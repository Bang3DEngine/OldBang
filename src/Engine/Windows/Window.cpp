#include "Bang/Window.h"

#include <SDL2/SDL.h>

#include "Bang/GL.h"
#include "Bang/Debug.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/GEngine.h"
#include "Bang/Texture2D.h"
#include "Bang/Application.h"
#include "Bang/AudioManager.h"
#include "Bang/SceneManager.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

Window::Window()
{
}

Window::~Window()
{
    delete m_input;
    delete m_gEngine;
    delete m_sceneManager;
    delete m_audioManager;
    SDL_DestroyWindow(m_sdlWindow);
}

void Window::Initialize()
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

    SDL_GLContext sharedContext = Application::GetInstance()->GetSharedGLContext();
    if (!sharedContext) { sharedContext = SDL_GL_CreateContext(m_sdlWindow); }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    // SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_MakeCurrent(m_sdlWindow, sharedContext);

    // Init GLEW
    glewExperimental = true;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        Debug_Error("Glew init error: " << glewGetErrorString(glewError));
    }
    GL::Enable(GL::Test::Depth);
    GL::Enable(GL::Test::Stencil);
    GL::Enable(GL::Test::CullFace);

    m_input               = new Input();
    m_sceneManager        = new SceneManager();
    m_audioManager        = new AudioManager();
    m_gEngine             = new GEngine();
    m_screenRenderTexture = new Texture2D();
}

void Window::SwapBuffers() const
{
    SDL_GL_SwapWindow(m_sdlWindow);
}

bool Window::MainLoopIteration()
{
    GL::SetViewport(0, 0, GetWidth(), GetHeight());
    GetInput()->ProcessEnqueuedEvents();

    GetSceneManager()->Update();
    Scene *activeScene = GetSceneManager()->GetActiveScene();
    UILayoutManager::ForceRebuildLayout(activeScene);
    if (activeScene) { GetGEngine()->Render(activeScene); }

    activeScene->GetUILayoutManager()->OnFrameFinished(activeScene);
    GetInput()->OnFrameFinished();

    GetGEngine()->RenderToScreen( GetScreenRenderTexture() );

    SwapBuffers();

    return true;
}

bool Window::HandleEvent(const SDL_Event &sdlEvent)
{
    GetInput()->PeekEvent(sdlEvent, this);

    switch (sdlEvent.type)
    {
        case SDL_WINDOWEVENT:
        if (sdlEvent.window.windowID == GetSDLWindowID())
        {
            switch (sdlEvent.window.event)
            {
                case SDL_WINDOWEVENT_CLOSE:
                    return false;

                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    OnResize(sdlEvent.window.data1, sdlEvent.window.data2);
                    break;
            }
        }
    }
    return true;
}

void Window::Resize(int w, int h)
{
    SDL_SetWindowSize(m_sdlWindow, w, h);
}

void Window::OnResize(int newWidth, int newHeight)
{
    GL::SetViewport(0, 0, GetWidth(), GetHeight());

    GetScreenRenderTexture()->Resize(newWidth, newHeight);
    GetGEngine()->Resize(newWidth, newHeight);

    Scene *activeScene = GetSceneManager()->GetActiveScene();
    if (activeScene) { UILayoutManager::ForceRebuildLayout(activeScene); }
}

int Window::GetWidth() const { return GetSize().x; }
int Window::GetHeight() const { return GetSize().y; }

bool Window::HasFocus() const
{
    return (SDL_GetWindowFlags(GetSDLWindow()) & SDL_WINDOW_INPUT_FOCUS) > 0;
}

bool Window::IsMouseOver() const
{
    return (SDL_GetWindowFlags(GetSDLWindow()) & SDL_WINDOW_MOUSE_FOCUS) > 0;
}

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
    return Window::GetCurrent()->GetHeight();
}

int Window::GetWidthS()
{
    return Window::GetCurrent()->GetWidth();
}

GEngine *Window::GetGEngine() const
{
    return m_gEngine;
}

Input *Window::GetInput() const
{
    return m_input;
}

AudioManager *Window::GetAudioManager() const
{
    return m_audioManager;
}

SceneManager *Window::GetSceneManager() const
{
    return m_sceneManager;
}

SDL_Window *Window::GetSDLWindow() const
{
    return m_sdlWindow;
}

uint Window::GetSDLWindowID() const
{
    return SDL_GetWindowID(m_sdlWindow);
}

Window *Window::GetCurrent()
{
    Application *app = Application::GetInstance();
    return app ? app->GetCurrentWindow() : nullptr;
}
