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
#include "Bang/BehaviourManager.h"
#include "Bang/G_FontSheetCreator.h"

Application* Application::s_appSingleton = nullptr;

Application::Application()
{
    Application::s_appSingleton = this;

    m_window           = new Window();
    m_time             = new Time();
    m_input            = new Input();
    m_shaderManager    = new ShaderManager();
    m_audioManager     = new AudioManager();
    m_sceneManager     = new SceneManager();
    m_assetsManager    = new AssetsManager();
    m_behaviourManager = new BehaviourManager();
}

Application::~Application()
{
    delete m_time;
    delete m_input;
    delete m_audioManager;
    delete m_sceneManager;
    delete m_assetsManager;
    delete m_behaviourManager;
}

void Application::MainLoop()
{
    while (true)
    {
        float deltaTime = float(Time::GetNow() - m_lastRenderTime) / 1000.0f;
        m_lastRenderTime = Time::GetNow();
        Time::GetInstance()->m_deltaTime = deltaTime;
        Time::s_deltaTime = deltaTime;
        Time::s_time += deltaTime;
        //

        // Process mouse and key events, so the Input is available in OnUpdate
        // as accurate as possible.
        // Lost events in between Update and Render will be delayed by Input.
        Input::GetInstance()->ProcessEnqueuedEvents();

        SceneManager::Update();

        // Render screen
        Screen::GetInstance()->Render();

        Input::GetInstance()->OnFrameFinished();
    }
}

Time *Application::GetTime() const
{
    return m_time;
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

void Application::ResetDeltaTime()
{
    m_lastRenderTime = Time::GetNow();
}

ShaderManager *Application::GetShaderManager() const
{
    return m_shaderManager;
}
