#include "Bang/Application.h"

#include <thread>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Bang/GL.h"
#include "Bang/Math.h"
#include "Bang/Time.h"
#include "Bang/Scene.h"
#include "Bang/Paths.h"
#include "Bang/Chrono.h"
#include "Bang/Window.h"
#include "Bang/GEngine.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/AudioManager.h"
#include "Bang/DialogWindow.h"
#include "Bang/SceneManager.h"
#include "Bang/BehaviourManager.h"
#include "Bang/FontSheetCreator.h"
#include "Bang/ImportFilesManager.h"

USING_NAMESPACE_BANG

Application* Application::s_appSingleton = nullptr;

Application::Application(int argc, char **argv, const Path &engineRootPath)
{
    srand(1234);

    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) { Debug_Error("Failed to init SDL"); }
    if ( TTF_Init() )
    {
        Debug_Error("Could not init FreeType library: Error(" <<
                    TTF_GetError() <<  ")");
    }

    Application::s_appSingleton = this;

    m_time = new Time();
    m_paths = new Paths();
    m_paths->InitPaths(engineRootPath);

    m_behaviourManager   = new BehaviourManager();
    m_importFilesManager = new ImportFilesManager();

    ImportFilesManager::CreateMissingImportFiles();
    ImportFilesManager::LoadImportFilepathGUIDs();
}

Application::~Application()
{
    delete m_time;
    delete m_paths;
    delete m_behaviourManager;

    TTF_Quit();
}

Window *Application::CreateWindow()
{
    Window *w = _CreateWindow();
    SetupWindow(w);
    return w;
}

DialogWindow *Application::CreateDialogWindow(Window *parentWindow)
{
     DialogWindow *w = new DialogWindow(parentWindow);
     SetupWindow(w);
     return w;
}

void Application::SetupWindow(Window *window)
{
    BindWindow(window);
    m_windows.PushBack(window);

    window->Create(SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    window->OnResize(window->GetWidth(), window->GetHeight());
}

void Application::SetActiveWindow(Window *window)
{
    Window::SetActive(window);
}

void Application::BindWindow(Window *window)
{
    p_latestCurrentWindow = p_currentWindow;
    p_currentWindow = window;
    window->MakeCurrent();
}

Window *Application::GetTopWindow()
{
    Application *app = Application::GetInstance();
    if (!app) { return nullptr; }

    for (Window *w : app->m_windows)
    {
        if (w->HasFocus()) { return w; }
    }
    return nullptr;
}

#ifdef GPROF
#include <gperftools/profiler.h>
#endif
int Application::MainLoop()
{
    #ifdef GPROF
    Path profileOutFile = Paths::ExecutablePath().GetDirectory().Append("profiling_info.out");
    Debug_Log("Writing profiling information to: '" << profileOutFile << "'");
    ProfilerStart(profileOutFile.GetAbsolute().ToCString());
    #endif

    bool exit = false;
    while (!exit && !m_forcedExit)
    {
        exit = MainLoopIteration();
        #ifdef GPROF
        ProfilerFlush();
        #endif
    }

    #ifdef GPROF
    ProfilerStop();
    #endif

    return m_exitCode;
}

bool Application::MainLoopIteration()
{
    List<Window*> windows = GetWindows();
    for (Window *w : windows)
    {
        SetActiveWindow(w);
        BindWindow(w);
        w->MainLoopIteration();
        SetActiveWindow(nullptr);
    }

    bool exit = false;
    if (!HandleEvents())     { exit = true; }
    DestroyQueuedWindows();
    if (m_windows.IsEmpty()) { exit = true; }

    return exit;
}

void Application::BlockingWait(Window *win)
{
    Window *latestCurrentWindow = p_latestCurrentWindow;
    List<Window*> allWindows = m_windows;

    allWindows.Remove(win);
    m_windows = {win};
    BindWindow(win);

    MainLoop();

    BindWindow(latestCurrentWindow);
    m_windows = allWindows;
}

bool Application::HandleEvents()
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
                    SetActiveWindow(w);
                    p_currentWindow = w;
                    bool hasNotClosed = w->HandleEvent(sdlEvent);
                    if (!hasNotClosed)
                    {
                        delete w;
                        itw = m_windows.Remove(itw);
                    }
                    else { ++itw; }
                    SetActiveWindow(nullptr);
                }
        }
    }

    for (Window *w : GetWindows())
    {
        BindWindow(w);
        w->OnHandleEventsFinished();
        SetActiveWindow(nullptr);
    }

    return true;
}

Time *Application::GetTime() const
{
    return m_time;
}

Paths *Application::GetPaths() const
{
    return m_paths;
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
        app->m_forcedExit = true;
        app->m_exitCode = returnCode;
    }
}

Window *Application::_CreateWindow() { return new Window(); }

void Application::DestroyWindow(Window *window)
{
    p_windowsToBeDestroyed.PushBack(window);
}
void Application::DestroyQueuedWindows()
{
    Window *latestWindow = Window::GetActive();
    for (Window *w : p_windowsToBeDestroyed)
    {
        Window::SetActive(w);
        m_windows.Remove(w);
        delete w;
    }
    p_windowsToBeDestroyed.Clear();
    Window::SetActive(latestWindow);
}

Window *Application::GetCurrentWindow() const
{
    return p_currentWindow;
}

SceneManager *Application::CreateSceneManager() const
{
    return new SceneManager();
}

const List<Window *> &Application::GetWindows() const
{
    return m_windows;
}
