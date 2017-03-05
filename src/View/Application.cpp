#include "Application.h"

#include <QThreadPool>

#ifdef BANG_EDITOR
#include "EditorScene.h"
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
#include "AudioManager.h"
#include "SceneManager.h"
#include "AssetsManager.h"
#include "BehaviourManager.h"
#include "FontSheetCreator.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
}

void Application::StartEditor()
{
    QThreadPool::globalInstance()->setMaxThreadCount(256);

    m_audioManager = new AudioManager();
    m_sceneManager = new SceneManager();
    m_assetsManager = new AssetsManager();
    m_behaviourManager = new BehaviourManager();

    m_drawTimer.start(c_redrawDelay);
    connect(&m_drawTimer, SIGNAL(timeout()), this, SLOT(OnDrawTimerTick()));

    m_lastRenderTime = Time::GetNow();
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
    Time::s_time += deltaTime;
    //

    // Process mouse and key events, so the Input is available in OnUpdate
    // as accurate as possible.
    // Lost events in between Update and Render will be delayed by Input.
    processEvents();
    Input::GetInstance()->ProcessEnqueuedEvents();

    SceneManager::TryToLoadQueuedScene();
    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        m_lastRenderTime = Time::GetNow();
        activeScene->_OnUpdate();
    }

    // Render screen
    Screen::GetInstance()->Render();
    Screen::GetInstance()->swapBuffers();

    Input::GetInstance()->OnFrameFinished();

    #ifdef BANG_EDITOR
    Console::GetInstance()->ProcessMessagesQueue();
    #endif
}

AssetsManager *Application::GetAssetsManager() const
{
    return m_assetsManager;
}

Application *Application::GetInstance()
{
    #ifdef BANG_EDITOR
    ASSERT(EditorWindow::GetInstance(), "", return nullptr);
    return Object::SCast<Application>(
                EditorWindow::GetInstance()->GetApplication());
    #else
    ASSERT(GameWindow::GetInstance(), "", return nullptr);
    return Object::SCast<Application>(
                GameWindow::GetInstance()->GetApplication());
    #endif
}

bool Application::notify(QObject *receiver, QEvent *e)
{
    Screen *screen = Screen::GetInstance();
    if (screen && receiver == screen)
    {
        Input::GetInstance()->EnqueueEvent(e);
    }

    #ifdef BANG_EDITOR
    if (e->type() == QEvent::MouseButtonPress)
    {
        DragDropManager::HandleGlobalMousePress(receiver, e);
    }
    else if (e->type() == QEvent::MouseButtonRelease)
    {
        DragDropManager::HandleGlobalMouseRelease(receiver, e);
    }

    ShortcutManager *sm = ShortcutManager::GetInstance();
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *ev = Object::SCast<QKeyEvent>(e);
        m_lastKeyPressEvInfo.time = Time::GetNow();
        m_lastKeyPressEvInfo.key = ev->key();
        if (!ev->isAutoRepeat())
        {
            sm->OnKeyPressed( Input::Key(ev->key()) );
        }
    }
    else if (e->type() == QEvent::KeyRelease)
    {
        QKeyEvent *ev = Object::SCast<QKeyEvent>(e);
        if (!ev->isAutoRepeat())
        {
            sm->OnKeyReleased( Input::Key(ev->key()) );
        }
    }

    if (e->type() == QEvent::Shortcut ||
        e->type() == QEvent::WindowDeactivate ||
        e->type() == QEvent::ApplicationDeactivate)
    {
        sm->Clear();
    }
    #endif

    return QApplication::notify(receiver, e);
}

bool Application::CurrentKeyReleaseIsAutoRepeat(const QKeyEvent *keyReleaseEvent)
{
    return m_lastKeyPressEvInfo.key == keyReleaseEvent->key() &&
           Time::GetNow() == m_lastKeyPressEvInfo.time;
}
