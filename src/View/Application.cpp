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

#include "Math.h"
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

void Application::InitManagers()
{
    m_audioManager = new AudioManager();
    m_sceneManager = new SceneManager();
    m_assetsManager = new AssetsManager();
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

        Chrono c;

        // Update deltaTime
        float deltaTime = float(Time::GetNow() - m_lastRenderTime) / 1000.0f;
        m_lastRenderTime = Time::GetNow();
        Time::GetInstance()->m_deltaTime = deltaTime;
        Time::s_deltaTime = deltaTime;
        Time::s_time += deltaTime;
        //

        // Process mouse and key events, so the Input is available in OnUpdate
        // as accurate as possible.
        // Lost events in between Update and Render will be delayed by Input.
        //processEvents();
        c.MarkEvent("Input::ProcessEnqueuedEvents()");
        Input::GetInstance()->ProcessEnqueuedEvents();

        SceneManager::TryToLoadQueuedScene();
        Scene *activeScene = SceneManager::GetActiveScene();
        if (activeScene)
        {
            c.MarkEvent("activeScene->_OnUpdate");
            activeScene->_OnUpdate();
        }

        // Render screen
        c.MarkEvent("Render()");
        Screen::GetInstance()->Render();
        c.MarkEvent("swapBuffers()");
        Screen::GetInstance()->swapBuffers();

        c.MarkEvent("Input::OnFrameFinished()");
        Input::GetInstance()->OnFrameFinished();

        #ifdef BANG_EDITOR
        c.MarkEvent("Console::ProcessMessagesQueue()");
        Console::GetInstance()->ProcessMessagesQueue();
        #endif

        c.Log();
    }
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
