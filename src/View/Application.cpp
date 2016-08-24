#include "Application.h"

#ifdef BANG_EDITOR
#include "EditorWindow.h"
#include "DragDropManager.h"
#include "ShortcutManager.h"
#include "WindowEventManager.h"
#else
#include "GameWindow.h"
#endif

#include "Scene.h"
#include "Screen.h"
#include "SceneManager.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
    m_sceneManager = new SceneManager();

    connect(&m_drawTimer, SIGNAL(timeout()), this, SLOT(OnDrawTimerTick()));

    m_drawTimer.setInterval(c_redrawDelay);
    m_drawTimer.start();

    m_lastRenderTime = Time::GetNow();
}

void Application::OnDrawTimerTick()
{
    // PAINTGL CALLED HERE OR BELOW (Because Qt reasons)

    m_delayEventsForNextFrame = false;

    // Update deltaTime
    float deltaTime = float(Time::GetNow() - m_lastRenderTime) / 1000.0f;
    Time::GetInstance()->m_deltaTime = deltaTime;

    // Update mainBinary static deltaTime, for internal engine use
    // (this is not Behaviours' static deltaTime, theirs is updated in Behaviour _OnUpdate)
    Time::s_deltaTime = deltaTime;
    //

    // Process mouse and key events, so the Input is available in OnUpdate.
    // If we don't do this, events can happen between OnUpdate and Input->OnNewFrame()
    // And they would get lost.
    Debug_Log("Before process events");
    processEvents();
    Debug_Log("After process events");

    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        m_lastRenderTime = Time::GetNow();
        activeScene->_OnUpdate();
    }


    // Render screen  (_OnRender mainly)
    // Screen::GetInstance()->update(); // update() calls Screen's paintGL method

    Input::GetInstance()->OnNewFrame(); // Notify newFrame has passed
    m_delayEventsForNextFrame = true;

    // PAINTGL CALLED HERE OR ABOVE (Because Qt reasons)
    Debug_Log("_________________________");
}


Application *Application::GetInstance()
{
    #ifdef BANG_EDITOR
    return static_cast<Application*>(EditorWindow::GetInstance()->GetApplication());
    #else
    return static_cast<Application*>(GameWindow::GetInstance()->GetApplication());
    #endif
}

bool Application::notify(QObject *receiver, QEvent *e)
{
    #ifdef BANG_EDITOR
    if (e->type() == QEvent::MouseButtonPress)
    {
        DragDropManager::HandleGlobalMousePress(receiver, e);
    }
    else if (e->type() == QEvent::MouseButtonRelease)
    {
        DragDropManager::HandleGlobalMouseRelease(receiver, e);
    }

    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *ev = static_cast<QKeyEvent*>(e);
        if (!ev->isAutoRepeat())
        {
            ShortcutManager::GetInstance()->OnKeyPressed( Input::Key(ev->key()) );
        }
    }
    else if (e->type() == QEvent::KeyRelease)
    {
        QKeyEvent *ev = static_cast<QKeyEvent*>(e);
        if (!ev->isAutoRepeat())
        {
            ShortcutManager::GetInstance()->OnKeyReleased( Input::Key(ev->key()) );
        }
    }

    if (e->type() == QEvent::Shortcut)
    {
        ShortcutManager::GetInstance()->Clear();
    }
    #endif

    return QApplication::notify(receiver, e);
}

bool Application::DelayEventsForNextFrame() const
{
    return m_delayEventsForNextFrame;
}
