#include "Bang/Application.h"

#include <thread>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Bang/GL.h"
#include "Bang/Math.h"
#include "Bang/Time.h"
#include "Bang/Debug.h"
#include "Bang/Input.h"
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
#include "Bang/ComponentFactory.h"
#include "Bang/FontSheetCreator.h"
#include "Bang/ImportFilesManager.h"

USING_NAMESPACE_BANG

Application* Application::s_appSingleton = nullptr;

Application::Application()
{
}

void Application::Init(const Path &engineRootPath)
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
    m_paths = CreatePaths();
    m_paths->InitPaths(engineRootPath);
    m_componentFactory = CreateComponentFactory();

    m_audioManager = new AudioManager();
    m_audioManager->Init();

    m_importFilesManager = new ImportFilesManager();

    ImportFilesManager::CreateMissingImportFiles(Paths::GetEngineAssetsDir());
    ImportFilesManager::LoadImportFilepathGUIDs(Paths::GetEngineAssetsDir());
}


Application::~Application()
{
    delete m_time;
    delete m_paths;
    delete m_audioManager;
    delete m_importFilesManager;

    for (Window *w : m_windows) { delete w; }
    for (Window *w : p_windowsToBeDestroyed) { delete w; }

    TTF_Quit();
    SDL_Quit();
}

Window *Application::CreateWindow()
{
    Window *w = _CreateWindow();
    SetupWindow(w);
    return w;
}

DialogWindow *Application::CreateDialogWindow(Window *parentWindow,
                                              bool resizable)
{
     DialogWindow *w = new DialogWindow(parentWindow, resizable);
     SetupWindow(w);
     return w;
}

void Application::SetupWindow(Window *window)
{
    m_windows.PushBack(window);

    window->Create(SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    window->OnResize(window->GetWidth(), window->GetHeight());
}

Window *Application::GetMainWindow()
{
    Application *app = Application::GetInstance();
    if (!app) { return nullptr; }

    return !app->m_windows.IsEmpty() ? app->m_windows.Front() : nullptr;
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
        Window::SetActive(w);
        w->MainLoopIteration();
        Window::SetActive(nullptr);
    }

    bool exit = false;
    if (!HandleEvents())     { exit = true; }
    if (m_windows.IsEmpty()) { exit = true; }
    DestroyQueuedWindows();

    return exit;
}

void Application::BlockingWait(Window *win, Window *previousWindow)
{
    List<Window*> allWindows = m_windows;
    allWindows.Remove(win);

    m_windows = {win};
    Window::SetActive(win);

    MainLoop();

    Window::SetActive(previousWindow);
    m_windows = allWindows;
}

bool Application::HandleEvents()
{
    SDL_Event sdlEvent;
    constexpr int AreThereMoreEvents = 1;
    while (SDL_PollEvent(&sdlEvent) == AreThereMoreEvents)
    {
        switch (sdlEvent.type)
        {
            default:
            {
                List<Window*> windowsToBeClosed;
                for (Window *w : m_windows)
                {
                    Window::SetActive(w);
                    bool hasNotClosed = w->HandleEvent(sdlEvent);
                    if (!hasNotClosed) { windowsToBeClosed.PushBack(w); }
                    Window::SetActive(nullptr);
                }

                for (Window *w : windowsToBeClosed)
                {
                    Window::SetActive(w);
                    w->OnClosed();
                    Window::SetActive(nullptr);

                    Window::SetActive(w);
                    m_windows.Remove(w);
                    delete w;
                    Window::SetActive(nullptr);
                }
            }
        }
    }

    for (Window *w : GetWindows())
    {
        Window::SetActive(w);
        w->OnHandleEventsFinished();
        Window::SetActive(nullptr);
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

AudioManager *Application::GetAudioManager() const
{
    return m_audioManager;
}

ImportFilesManager *Application::GetImportFilesManager() const
{
    return m_importFilesManager;
}

ComponentFactory *Application::GetComponentFactory() const
{
    return m_componentFactory;
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
    if (immediate) { std::quick_exit(returnCode); }
    else
    {
        Application *app = Application::GetInstance();
        app->m_forcedExit = true;
        app->m_exitCode = returnCode;
    }
}

Paths *Application::CreatePaths()
{
    return new Paths();
}

Window *Application::_CreateWindow() { return new Window(); }

ComponentFactory *Application::CreateComponentFactory()
{
    return new ComponentFactory();
}

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

SceneManager *Application::CreateSceneManager() const
{
    return new SceneManager();
}

const List<Window *> &Application::GetWindows() const
{
    return m_windows;
}
