#include "Bang/Application.h"

#include <thread>

#include <SDL2/SDL_ttf.h>

#include "Bang/Math.h"
#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Paths.h"
#include "Bang/Screen.h"
#include "Bang/Chrono.h"
#include "Bang/Window.h"
#include "Bang/Screen.h"
#include "Bang/Resources.h"
#include "Bang/AudioManager.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderManager.h"
#include "Bang/BehaviourManager.h"
#include "Bang/FontSheetCreator.h"
#include "Bang/ImportFilesManager.h"

Application* Application::s_appSingleton = nullptr;

Application::Application(int argc, char **argv, const Path &engineRootPath)
{
    srand(time(NULL));

    Application::s_appSingleton = this;

    m_paths = new Paths();
    m_paths->InitPaths(engineRootPath);

    m_input              = new Input();
    m_time               = new Time();
    m_sceneManager       = new SceneManager();
    m_shaderManager      = new ShaderManager();
    m_audioManager       = new AudioManager();
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
    delete m_paths;
    delete m_input;
    delete m_window;
    delete m_shaderManager;
    delete m_audioManager;
    delete m_sceneManager;
    delete m_behaviourManager;
    delete m_resources;

    TTF_Quit();
}

void Application::CreateWindow()
{
    m_window = new Window();
}

int Application::MainLoop()
{
    bool quit = false;
    while (!quit && !m_exit)
    {
        m_time->OnFrameStarted();

        // Process mouse and key events, so the Input is available in OnUpdate
        // as accurate as possible.
        // Lost events in between Update and Render will be delayed by Input.
        m_input->ProcessEnqueuedEvents();
        m_sceneManager->Update();
        if (m_window) { m_window->Render(); }

        m_input->OnFrameFinished();
        m_time->OnFrameFinished();

        quit = !ProcessEvents();

        SDL_Delay(RedrawDelay_ms);
    }
    return m_exitCode;
}

bool Application::ProcessEvents()
{
    SDL_Event event;
    constexpr int THERE_ARE_MORE_EVENTS = 1;
    while (SDL_PollEvent(&event) == THERE_ARE_MORE_EVENTS)
    {
        m_input->PeekEvent(event);
        switch (event.type)
        {
            case SDL_QUIT: return false;

            case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
                case SDL_WINDOWEVENT_CLOSE:
                    return false;

                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    m_window->OnResize(event.window.data1, event.window.data2);
                    break;
            }
            break;
        }
    }
    return true;
}

Input *Application::GetInput() const
{
    return m_input;
}

Time *Application::GetTime() const
{
    return m_time;
}

Paths *Application::GetPaths() const
{
    return m_paths;
}

SceneManager *Application::GetSceneManager() const
{
    return m_sceneManager;
}

AudioManager *Application::GetAudioManager() const
{
    return m_audioManager;
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

Window *Application::GetMainWindow() const
{
    return m_window;
}

ShaderManager *Application::GetShaderManager() const
{
    return m_shaderManager;
}
