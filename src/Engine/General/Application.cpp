#include "Bang/Application.h"

#include <thread>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Bang/GL.h"
#include "Bang/Math.h"
#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Paths.h"
#include "Bang/Chrono.h"
#include "Bang/Window.h"
#include "Bang/GEngine.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/AudioManager.h"
#include "Bang/SceneManager.h"
#include "Bang/BehaviourManager.h"
#include "Bang/FontSheetCreator.h"
#include "Bang/ImportFilesManager.h"

USING_NAMESPACE_BANG

Application* Application::s_appSingleton = nullptr;

Application::Application(int argc, char **argv, const Path &engineRootPath)
{
    srand(time(NULL));

    Application::s_appSingleton = this;

    m_time = new Time();
    m_paths = new Paths();
    m_paths->InitPaths(engineRootPath);

    m_resources          = new Resources();
    m_behaviourManager   = new BehaviourManager();
    m_importFilesManager = new ImportFilesManager();

    ImportFilesManager::CreateMissingProjectImportFiles();
    ImportFilesManager::LoadImportFilepathGUIDs();

    if ( TTF_Init() )
    {
        Debug_Error("Could not init FreeType library: Error(" <<
                    TTF_GetError() <<  ")");
    }
}

Application::~Application()
{
    delete m_time;
    delete m_paths;
    delete m_behaviourManager;
    delete m_resources;

    TTF_Quit();
}

Window* Application::CreateWindow()
{
    Window *window = new Window();
    window->Initialize();
    if (!m_sdlGLSharedContext) { m_sdlGLSharedContext = SDL_GL_GetCurrentContext(); }

    p_currentWindow = window;
    m_windows.PushBack(window);
    window->OnResize(window->GetWidth(), window->GetHeight());

    return window;
}

void Application::RemoveWindow(Window *window)
{
    for (Window *w : GetWindows()) { if (w == window) { delete w; break; } }
    m_windows.Remove(window);
}

int Application::MainLoop()
{
    while (!m_exit)
    {
        GetTime()->OnFrameStarted();
        for (Window *w : GetWindows())
        {
            p_currentWindow = w;
            SDL_GL_MakeCurrent(w->GetSDLWindow(), GetSharedGLContext());
            w->MainLoopIteration();
        }
        GetTime()->OnFrameFinished();
        SDL_Delay(RedrawDelay_ms);

        if (!ProcessEvents())    { m_exit = true; }
        if (m_windows.IsEmpty()) { m_exit = true; }
    }
    return m_exitCode;
}

bool Application::ProcessEvents()
{
    SDL_Event sdlEvent;
    constexpr int THERE_ARE_MORE_EVENTS = 1;
    while (SDL_PollEvent(&sdlEvent) == THERE_ARE_MORE_EVENTS)
    {
        switch (sdlEvent.type)
        {
            case SDL_QUIT: return false;

            default:
                for (auto itw = m_windows.Begin(); itw != m_windows.End(); )
                {
                    Window *w = *itw;
                    bool stillOpen = w->HandleEvent(sdlEvent);
                    if (!stillOpen)
                    {
                        delete w;
                        itw = m_windows.Remove(itw);
                    }
                    else { ++itw; }
                }
        }
    }
    return true;
}

SDL_GLContext Application::GetSharedGLContext() const
{
    return m_sdlGLSharedContext;
}

Time *Application::GetTime() const
{
    return m_time;
}

Paths *Application::GetPaths() const
{
    return m_paths;
}

Resources *Application::GetResources() const
{
    return m_resources;
}

ImportFilesManager *Application::GetImportFilesManager() const
{
    return m_importFilesManager;
}

BehaviourManager *Application::GetBehaviourManager() const
{
    return m_behaviourManager;
}

Application *Application::GetInstance()
{
    return Application::s_appSingleton;
}

void Application::SetApplicationSingleton(Application *app)
{
    Application::s_appSingleton = app;
}

void Application::Exit(int returnCode, bool immediate)
{
    if (immediate) { exit(returnCode); }
    else
    {
        Application *app = Application::GetInstance();
        app->m_exit = true;
        app->m_exitCode = returnCode;
    }
}

Window *Application::GetCurrentWindow() const
{
    return p_currentWindow ? p_currentWindow :
             ((m_windows.Size() >= 1) ? m_windows.Front() : nullptr);
}

const List<Window *> &Application::GetWindows() const
{
    return m_windows;
}
