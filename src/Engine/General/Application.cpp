#include "Bang/Application.h"

#include <thread>

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
#include "Bang/Settings.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/AudioManager.h"
#include "Bang/DialogWindow.h"
#include "Bang/SceneManager.h"
#include "Bang/WindowManager.h"
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

    Application::s_appSingleton = this;

    m_time = new Time();

    m_paths = CreatePaths();
    m_paths->InitPaths(engineRootPath);

    m_settings = CreateSettings();

    m_audioManager = new AudioManager();
    m_audioManager->Init();

    m_windowManager = new WindowManager();
    GetWindowManager()->Init();

    m_importFilesManager = new ImportFilesManager();
    ImportFilesManager::CreateMissingImportFiles(Paths::GetEngineAssetsDir());
    ImportFilesManager::LoadImportFilepathGUIDs(Paths::GetEngineAssetsDir());
}


Application::~Application()
{
    delete m_time;
    delete m_paths;
    delete m_settings;
    delete m_audioManager;
    delete m_windowManager;
    delete m_importFilesManager;
}

#ifdef GPROF
#include <gperftools/profiler.h>
#endif
void InitProfiling()
{
    #ifdef GPROF
    Path profileOutFile = Paths::GetExecutablePath().GetDirectory().Append("profiling_info.out");
    Debug_Log("Writing profiling information to: '" << profileOutFile << "'");
    ProfilerStart(profileOutFile.GetAbsolute().ToCString());
    #endif
}
void FlushProfiling()
{
    #ifdef GPROF
    ProfilerFlush();
    #endif
}
void StopProfiling()
{
    #ifdef GPROF
    ProfilerStop();
    #endif
}

int Application::MainLoop()
{
    InitProfiling();
    Time::SetDeltaTimeReferenceToNow();

    bool exit = false;
    while (!exit && !m_forcedExit)
    {
        exit = MainLoopIteration();
        FlushProfiling();
    }

    StopProfiling();
    return m_exitCode;
}

bool Application::MainLoopIteration()
{
    bool exit = GetWindowManager()->MainLoopIteration();
    Thread::SleepCurrentThread(0.01f);
    return exit;
}

void Application::BlockingWait(Window *win)
{
    GetWindowManager()->OnBlockingWaitBegin(win);
    MainLoop();
    GetWindowManager()->OnBlockingWaitEnd();
}

Time *Application::GetTime() const
{
    return m_time;
}

Paths *Application::GetPaths() const
{
    return m_paths;
}

Settings *Application::GetSettings() const
{
    return m_settings;
}

AudioManager *Application::GetAudioManager() const
{
    return m_audioManager;
}

WindowManager *Application::GetWindowManager() const
{
    return m_windowManager;
}

ImportFilesManager *Application::GetImportFilesManager() const
{
    return m_importFilesManager;
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

Settings *Application::CreateSettings()
{
    return new Settings();
}

SceneManager *Application::CreateSceneManager() const
{
    return new SceneManager();
}
