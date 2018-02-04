
#include "Bang/Window.h"

#include <SDL2/SDL.h>

#include "Bang/GL.h"
#include "Bang/Debug.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/GEngine.h"
#include "Bang/GBuffer.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/UILayoutManager.h"
#include "Bang/SelectionFramebuffer.h"

USING_NAMESPACE_BANG

Window* Window::s_activeWindow = nullptr;

Window::Window()
{
}

Window::~Window()
{
    SetParent(nullptr);

    delete m_sceneManager;  m_sceneManager  = nullptr;
    delete m_input;         m_input         = nullptr;
    delete m_gEngine;       m_gEngine       = nullptr;

    m_resources->Destroy();
    delete m_resources;     m_resources     = nullptr;

    SDL_GL_DeleteContext(GetGLContext());
    SDL_DestroyWindow(m_sdlWindow);
}

void Window::Create(uint flags)
{
    Vector2i winSize(512);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_sdlWindow = SDL_CreateWindow("Bang",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   winSize.x,
                                   winSize.y,
                                   flags);

    m_sdlGLContext = SDL_GL_CreateContext(GetSDLWindow());
    Window::SetActive(this);

    SetMinSize(1, 1);
    SetMaxSize(99999, 99999);

    // Init GLEW
    glewExperimental = true;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        Debug_Error("Glew init error: " << glewGetErrorString(glewError));
    }

    m_input               = new Input();
    m_resources           = CreateResources();
    m_sceneManager        = CreateSceneManager();
    m_gEngine             = new GEngine();

    m_sceneManager->Init();
    m_gEngine->Init();

    SetSize(winSize.x, winSize.y);
}

void Window::SwapBuffers() const
{
    SDL_GL_SwapWindow( GetSDLWindow() );
}

void Window::MakeCurrent()
{
    SDL_GL_MakeCurrent(GetSDLWindow(), GetGLContext());
    GL::SetViewport(0, 0, GetWidth(), GetHeight());
}

bool Window::MainLoopIteration()
{
    RetrieveTitleBarHeight();

    Window::SetActive(this);
    GetInput()->ProcessEnqueuedEvents();

    Update();
    Time::SetDeltaTimeReferenceToNow();
    Render();

    GetInput()->OnFrameFinished();
    SwapBuffers();

    return true;
}

void Window::Clear()
{
    GL::ClearDepthBuffer();
    GL::ClearColorBuffer(Color::Zero);
}

void Window::Update()
{
    GetSceneManager()->Update();
}

void Window::Render()
{
    Clear();

    Scene *scene = GetSceneManager()->GetActiveScene();
    if (scene)
    {
        Camera *camera = scene->GetCamera();
        if (!camera) { return; }
        GetGEngine()->Render(scene, camera);
        BlitToWindow(camera);
    }
}

void Window::BlitToWindow(Camera *camera)
{
    if (camera)
    {
        Recti prevVP = GL::GetViewportRect();
        camera->SetViewportForBlitting();
        GetGEngine()->RenderGBufferColorToViewport(camera);
        GL::SetViewport(prevVP);
    }
}

bool Window::HandleEvent(const SDL_Event &sdlEvent)
{
    if (!IsBlockedByChildren())
    {
        GetInput()->PeekEvent(sdlEvent, this);
    }

    switch (sdlEvent.type)
    {
        case SDL_WINDOWEVENT:
        if (sdlEvent.window.windowID == GetSDLWindowID())
        {
            if (!IsBlockedByChildren())
            {
                switch (sdlEvent.window.event)
                {
                    case SDL_WINDOWEVENT_CLOSE: return false;
                }
            }

            switch (sdlEvent.window.event)
            {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                case SDL_WINDOWEVENT_RESIZED:
                    m_newSize = Vector2i(sdlEvent.window.data1,
                                         sdlEvent.window.data2);
                    break;
            }
        }
        else
        {
            // Other window (not this) events
            switch (sdlEvent.window.event)
            {
                case SDL_WINDOWEVENT_EXPOSED:
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    if (IsParentWindow(sdlEvent.window.windowID))
                    {
                        MoveToFront();
                    }
                    break;
            }
        }
    }

    return true;
}

void Window::OnHandleEventsFinished()
{
    OnResize(m_newSize.x, m_newSize.y);
}

void Window::OnClosed()
{

}

void Window::Maximize()
{
    SDL_MaximizeWindow( GetSDLWindow() );
}

void Window::Restore()
{
    SDL_RestoreWindow( GetSDLWindow() );
}

void Window::Minimize()
{
    SDL_MinimizeWindow( GetSDLWindow() );
}

void Window::MoveToFront()
{
    SDL_RaiseWindow(GetSDLWindow());
    for (Window *childWindow : p_children) { childWindow->MoveToFront(); }
}

void Window::SetBordered(bool bordered)
{
    SDL_SetWindowBordered(GetSDLWindow(), Cast<SDL_bool>(bordered));
}

void Window::SetMinSize(int minSizeX, int minSizeY)
{
    m_minSize = Vector2i(minSizeX, minSizeY);
    _SetMinSize(minSizeX, minSizeY);
}

void Window::SetMaxSize(int maxSizeX, int maxSizeY)
{
    m_maxSize = Vector2i(maxSizeX, maxSizeY);
    _SetMaxSize(maxSizeX, maxSizeY);
}

void Window::SetResizable(bool resizable)
{
    if (m_isResizable == resizable) { return; }
    m_isResizable = resizable;

    if (IsResizable())
    {
        _SetMaxSize(GetMaxSize().x, GetMaxSize().y);
        _SetMinSize(GetMinSize().x, GetMinSize().y);
        SetSize(GetSize().x, GetSize().y);
    }
    else
    {
        Vector2i size = GetSize();
        _SetMinSize(size.x,   size.y);
        _SetMaxSize(size.x+1, size.y+1);
    }
}

void Window::SetPosition(int newPosX, int newPosY)
{
    SDL_SetWindowPosition(GetSDLWindow(), newPosX, newPosY);
}

void Window::SetTitle(const String &title)
{
    SDL_SetWindowTitle(GetSDLWindow(), title.ToCString());
}

void Window::SetSize(int w, int h)
{
    SDL_SetWindowSize(m_sdlWindow, w, h);
    m_newSize = Vector2i(w, h);
    OnResize(w, h);
}

void Window::OnResize(int newWidth, int newHeight)
{
    if (m_newSize != m_prevSize)
    {
        m_newSize  = Vector2i(newWidth, newHeight);
        m_prevSize = m_newSize;
        GL::SetViewport(0, 0, GetWidth(), GetHeight());

        Scene *scene = GetSceneManager()->GetActiveScene();
        if (scene) { scene->OnResize(newWidth, newHeight); }
    }
}

int Window::GetWidth() const { return GetSize().x; }
int Window::GetHeight() const { return GetSize().y; }

bool Window::HasFocus() const { return HasFlags(SDL_WINDOW_INPUT_FOCUS); }
bool Window::IsBordered() const { return !HasFlags(SDL_WINDOW_BORDERLESS); }
String Window::GetTitle() const
{
    return String(SDL_GetWindowTitle(GetSDLWindow()));
}
bool Window::IsMouseOver() const { return HasFlags(SDL_WINDOW_MOUSE_FOCUS); }

Vector2i Window::GetSize() const
{
    Vector2i size;
    SDL_GetWindowSize(m_sdlWindow, &size.x, &size.y);
    return size;
}

Vector2i Window::GetMinSize() const
{
    return m_minSize;
}

Vector2i Window::GetMaxSize() const
{
    return m_maxSize;
}

bool Window::IsResizable() const
{
    return m_isResizable;
}

Vector2i Window::GetPosition() const
{
    Vector2i wpos;
    SDL_GetWindowPosition(GetSDLWindow(), &wpos.x, &wpos.y);
    return wpos;
}

Vector2i Window::GetInsidePosition() const
{
    return GetPosition() + Vector2i(0, GetTitleBarHeight());
}

bool Window::IsBlockedByChildren() const
{
    return !p_children.IsEmpty();
}

bool Window::HasFlags(uint flags) const
{
    return (SDL_GetWindowFlags(GetSDLWindow()) & flags) > 0;
}

int Window::GetHeightS()
{
    return Window::GetActive()->GetHeight();
}

int Window::GetWidthS()
{
    return Window::GetActive()->GetWidth();
}

SDL_GLContext Window::GetGLContext() const
{
    return m_sdlGLContext;
}

Resources *Window::GetResources() const
{
    return m_resources;
}

GEngine *Window::GetGEngine() const
{
    return m_gEngine;
}

Input *Window::GetInput() const
{
    return m_input;
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

uint Window::GetTitleBarHeight() const
{
    return m_titleBarHeight;
}

Window *Window::GetParentWindow() const
{
    return p_parent;
}

SceneManager *Window::CreateSceneManager() const
{
    return new SceneManager();
}

void Window::Destroy(Window *win)
{
    Application::GetInstance()->DestroyWindow(win);
}

Window *Window::GetActive()
{
    return Window::s_activeWindow;
}

void Window::SetParent(Window *parentWindow)
{
    ASSERT(parentWindow != this);
    ASSERT(!p_children.Contains(parentWindow));

    if (p_parent != parentWindow)
    {
        if (p_parent)
        {
            p_parent->p_children.Remove(this);
            p_parent->SetResizable(true);
        }

        p_parent = parentWindow;

        if (p_parent)
        {
            p_parent->SetResizable(false);
            p_parent->p_children.PushBack(this);
        }
    }
}

void Window::RetrieveTitleBarHeight()
{
    if(Input::IsMouseInsideWindow())
    {
        Vector2i localCoords, globalCoords;
        SDL_GetMouseState(&localCoords.x, &localCoords.y);
        SDL_GetGlobalMouseState(&globalCoords.x, &globalCoords.y);
        m_titleBarHeight = globalCoords.y - GetPosition().y - localCoords.y;
    }
}

Resources *Window::CreateResources() const
{
    return new Resources();
}

void Window::_SetMinSize(int minSizeX, int minSizeY)
{
    SDL_SetWindowMinimumSize(GetSDLWindow(), minSizeX, minSizeY);
}

void Window::_SetMaxSize(int maxSizeX, int maxSizeY)
{
    SDL_SetWindowMaximumSize(GetSDLWindow(), maxSizeX, maxSizeY);
}

Vector2i Window::_GetMinSize() const
{
    Vector2i minSize;
    SDL_GetWindowMinimumSize(GetSDLWindow(), &minSize.x, &minSize.y);
    return minSize;
}

Vector2i Window::_GetMaxSize() const
{
    Vector2i maxSize;
    SDL_GetWindowMaximumSize(GetSDLWindow(), &maxSize.x, &maxSize.y);
    return maxSize;
}

bool Window::IsParentWindow(int sdlWindowId) const
{
    return p_parent ? (p_parent->GetSDLWindowID() == sdlWindowId ||
                       p_parent->IsParentWindow(sdlWindowId)) : false;
}

void Window::SetActive(Window *window)
{
    Window::s_activeWindow = window;
    GEngine::SetActive(window ? window->GetGEngine() : nullptr);
    if (window) { window->MakeCurrent(); }
}
