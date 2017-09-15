#include "Bang/Application.h"

#include <thread>

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

    m_paths = new Paths();
    m_paths->InitPaths(engineRootPath);

    m_input              = new Input();
    m_time               = new Time();
    m_sceneManager       = new SceneManager();
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
    delete m_gEngine;
    delete m_audioManager;
    delete m_sceneManager;
    delete m_behaviourManager;
    delete m_resources;

    TTF_Quit();
}

void Application::CreateWindow()
{
    m_window = new Window();
    m_gEngine = new GEngine();
    OnResize(m_window->GetWidth(), m_window->GetHeight());
}

int Application::MainLoop()
{
    bool quit = false;
    while (!quit && !m_exit)
    {
        quit = MainLoopIteration();

        Texture2D *screenRenderTexture = GetWindow()->GetScreenRenderTexture();
        GEngine::GetInstance()->RenderToScreen(screenRenderTexture);

        m_window->SwapBuffers();

        quit = !ProcessEvents() || quit;

        SDL_Delay(RedrawDelay_ms);
    }
    return m_exitCode;
}

bool Application::MainLoopIteration()
{
    m_time->OnFrameStarted();

    m_input->ProcessEnqueuedEvents();

    UpdateScene();
    RenderScene();

    m_input->OnFrameFinished();
    m_time->OnFrameFinished();

    return false;
}

void Application::UpdateScene()
{
    m_sceneManager->Update();
}

void Application::RenderScene()
{
    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene) { m_gEngine->Render(activeScene); }
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
                    OnResize(event.window.data1, event.window.data2);
                    break;
            }
            break;
        }
    }
    return true;
}

void Application::OnResize(int newWidth, int newHeight)
{
    GL::SetViewport(0, 0, newWidth, newHeight);

    m_window->OnResize(newWidth, newHeight);
    m_gEngine->Resize(newWidth, newHeight);

    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene) { activeScene->ParentLayoutChanged(); }
}

GEngine *Application::GetGEngine() const
{
    return m_gEngine;
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

Window *Application::GetWindow() const
{
    return m_window;
}
