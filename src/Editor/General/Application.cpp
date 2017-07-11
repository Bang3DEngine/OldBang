#include "Bang/Application.h"

#include <QThreadPool>
#include "Bang/WinUndef.h"

#include "Bang/Math.h"
#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Screen.h"
#include "Bang/Chrono.h"
#include "Bang/G_Screen.h"
#include "Bang/GameWindow.h"
#include "Bang/AudioManager.h"
#include "Bang/SceneManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/EditorPlayFlow.h"
#include "Bang/BehaviourManager.h"
#include "Bang/G_FontSheetCreator.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
}

void Application::InitManagers()
{
    m_audioManager     = new AudioManager();
    m_sceneManager     = new SceneManager();
    m_assetsManager    = new AssetsManager();
    m_behaviourManager = new BehaviourManager();
}

Application::~Application()
{
    delete m_audioManager;
    delete m_sceneManager;
    delete m_assetsManager;
    delete m_behaviourManager;
}

AudioManager *Application::GetAudioManager() const
{
    return m_audioManager;
}

void Application::MainLoop()
{
    IWindow *win = IWindow::GetInstance();
    while ( win->GetMainWindow()->isVisible() )
    {
        processEvents();
        if (!Screen::GetInstance()) { continue; }

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
        Screen::GetInstance()->makeCurrent();
        Screen::GetInstance()->swapBuffers();

        Input::GetInstance()->OnFrameFinished();

        OnMainLoopIterationEnd();
    }
}

AssetsManager *Application::GetAssetsManager() const
{
    return m_assetsManager;
}

Application *Application::GetInstance()
{
    if (!GameWindow::GetInstance()) { return nullptr; }
    return Object::SCast<Application>(
                GameWindow::GetInstance()->GetApplication());
}

void Application::ResetDeltaTime()
{
    m_lastRenderTime = Time::GetNow();
}

bool Application::notify(QObject *receiver, QEvent *e)
{
    Screen *screen = Screen::GetInstance();
    if (screen && receiver == screen)
    {
        Input::GetInstance()->EnqueueEvent(e);
    }

    Input *input = Input::GetInstance();
    if (input)
    {
        input->EnqueueEvent(e);
    }

    return QApplication::notify(receiver, e);
}

void Application::OnMainLoopIterationEnd()
{
}
