#include "Bang/Application.h"

#include "Bang/Math.h"
#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Screen.h"
#include "Bang/Chrono.h"
#include "Bang/Window.h"
#include "Bang/G_Screen.h"
#include "Bang/ShaderManager.h"
#include "Bang/AudioManager.h"
#include "Bang/SceneManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/G_FontSheetCreator.h"

Application* Application::s_appSingleton = nullptr;

Application::Application(int argc, char **argv)
{
    Application::s_appSingleton = this;

    Path binPath(argv[0]);
    Paths::SetEnginePath( binPath.GetDirectory().GetDirectory().GetDirectory() );
    Paths::SetEngineBinaryDir(binPath.GetDirectory());

    m_input            = new Input();
    m_shaderManager    = new ShaderManager();
    m_audioManager     = new AudioManager();
    m_sceneManager     = new SceneManager();
    m_assetsManager    = new AssetsManager();
    m_window           = new Window();
}

Application::~Application()
{
    delete m_input;
    delete m_audioManager;
    delete m_sceneManager;
    delete m_assetsManager;
}

void Application::MainLoop()
{
    bool quit = false;
    while (!quit)
    {
        Time::OnFrameStarted();

        // Process mouse and key events, so the Input is available in OnUpdate
        // as accurate as possible.
        // Lost events in between Update and Render will be delayed by Input.
        m_input->ProcessEnqueuedEvents();
        m_sceneManager->Update();
        m_window->Render();

        m_input->OnFrameFinished();
        Time::OnFrameFinished();
        quit = !ProcessEvents();
    }
}

bool Application::ProcessEvents()
{
    SDL_Event event;
    constexpr int THERE_ARE_MORE_EVENTS = 1;
    while (SDL_PollEvent(&event) == THERE_ARE_MORE_EVENTS)
    {
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

            default:
                m_input->PeekEvent(event);
        }
    }
    return true;
}

Input *Application::GetInput() const
{
    return m_input;
}

AudioManager *Application::GetAudioManager() const
{
    return m_audioManager;
}

AssetsManager *Application::GetAssetsManager() const
{
    return m_assetsManager;
}

Application *Application::GetInstance()
{
    return Application::s_appSingleton;
}

Window *Application::GetMainWindow() const
{
    return m_window;
}

ShaderManager *Application::GetShaderManager() const
{
    return m_shaderManager;
}
