#include "Application.h"

#ifdef BANG_EDITOR
#include "EditorWindow.h"
#include "DragDropManager.h"
#include "ShortcutManager.h"
#include "WindowEventManager.h"
#else
#include "GameWindow.h"
#endif

#include "Time.h"
#include "Input.h"
#include "Scene.h"
#include "Screen.h"
#include "Chrono.h"
#include "SceneManager.h"
#include "AssetsManager.h"
#include "BehaviourManager.h"
#include "FontSheetCreator.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
    m_sceneManager = new SceneManager();
    m_assetsManager = new AssetsManager();
    m_behaviourManager = new BehaviourManager();

    connect(&m_drawTimer, SIGNAL(timeout()), this, SLOT(OnDrawTimerTick()));

    m_drawTimer.start(c_redrawDelay);

    m_lastRenderTime = Time::GetNow();
}

void Application::OnDrawTimerTick()
{
    ASSERT(Screen::GetInstance());

    // Update deltaTime
    float deltaTime = float(Time::GetNow() - m_lastRenderTime) / 1000.0f;
    Time::GetInstance()->m_deltaTime = deltaTime;

    // Update mainBinary static deltaTime, for internal engine use
    // (this is not Behaviours' static deltaTime, theirs is
    //  updated in Behaviour _OnUpdate)
    Time::s_deltaTime = deltaTime;
    //

    // Process mouse and key events, so the Input is available in OnUpdate
    // as accurate as possible.
    // Lost events in between Update and Render will be delayed by Input.
    processEvents();
    Input::GetInstance()->ProcessEnqueuedEvents();

    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        m_lastRenderTime = Time::GetNow();
        activeScene->_OnUpdate();
    }

    Chrono c("FPS");
    // Render screen
    c.MarkEvent("Render");
    Screen::GetInstance()->Render();
    Screen::GetInstance()->swapBuffers();
    //c.Log();

    Input::GetInstance()->OnFrameFinished(); // Notify to Input that a new frame has passed
}

AssetsManager *Application::GetAssetsManager() const
{
    return m_assetsManager;
}

Application *Application::GetInstance()
{
    #ifdef BANG_EDITOR
    ASSERT(EditorWindow::GetInstance(), "", return nullptr);
    return static_cast<Application*>(EditorWindow::GetInstance()->GetApplication());
    #else
    ASSERT(GameWindow::GetInstance(), "", return nullptr);
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

    if (receiver == focusWidget())
    {
        if (e->type() == QEvent::KeyPress)
        {
            QKeyEvent *ev = dynamic_cast<QKeyEvent*>(e);
            m_lastKeyPressEvInfo.time = Time::GetNow();
            m_lastKeyPressEvInfo.key = ev->key();
            if (!ev->isAutoRepeat())
            {
                ShortcutManager::GetInstance()->OnKeyPressed( Input::Key(ev->key()) );
            }
        }
        else if (e->type() == QEvent::KeyRelease)
        {
            QKeyEvent *ev = dynamic_cast<QKeyEvent*>(e);
            if (!ev->isAutoRepeat())
            {
                ShortcutManager::GetInstance()->OnKeyReleased( Input::Key(ev->key()) );
            }
        }

        if (e->type() == QEvent::Shortcut)
        {
            ShortcutManager::GetInstance()->Clear();
        }
    }
    #endif

    //int eType = e->type();
    //std::cout << "Notifying " << eType << " to " << receiver << std::endl;
    return QApplication::notify(receiver, e);
}

bool Application::CurrentKeyReleaseIsAutoRepeat(const QKeyEvent *keyReleaseEvent)
{
    return m_lastKeyPressEvInfo.key == keyReleaseEvent->key() &&
           Time::GetNow() == m_lastKeyPressEvInfo.time;
}
