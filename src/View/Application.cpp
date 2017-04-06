#include "Bang/Application.h"

#include <QThreadPool>
#include "Bang/WinUndef.h"

#ifdef BANG_EDITOR
#include "Bang/EditorScene.h"
#include "Bang/EditorWindow.h"
#include "Bang/DragDropManager.h"
#include "Bang/WindowEventManager.h"
#else
#include "Bang/GameWindow.h"
#endif

#include "Bang/Math.h"
#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Screen.h"
#include "Bang/Chrono.h"
#include "Bang/AudioManager.h"
#include "Bang/SceneManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/BehaviourManager.h"
#include "Bang/FontSheetCreator.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
}

void Application::InitManagers()
{
    #ifdef BANG_EDITOR
    m_fileTracker      = new FileTracker();
    #endif

    m_audioManager     = new AudioManager();
    m_sceneManager     = new SceneManager();
    m_assetsManager    = new AssetsManager();
    m_behaviourManager = new BehaviourManager();
}

Application::~Application()
{
    #ifdef BANG_EDITOR
    delete m_fileTracker;
    #endif

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

        SceneManager::TryToLoadQueuedScene();
        Scene *activeScene = SceneManager::GetActiveScene();
        if (activeScene)
        {
            activeScene->_OnUpdate();
        }
        activeScene->DestroyQueuedGameObjects();

        // Render screen
        Screen::GetInstance()->Render();
		Screen::GetInstance()->makeCurrent();
        Screen::GetInstance()->swapBuffers();

        Input::GetInstance()->OnFrameFinished();

        #ifdef BANG_EDITOR
        Console::GetInstance()->ProcessMessagesQueue();
        #endif

        //c.Log();
    }
}

AssetsManager *Application::GetAssetsManager() const
{
    return m_assetsManager;
}

Application *Application::GetInstance()
{
    #ifdef BANG_EDITOR
	if (!EditorWindow::GetInstance()) { return nullptr; }
    return Object::SCast<Application>(
                EditorWindow::GetInstance()->GetApplication());
    #else
    if (!GameWindow::GetInstance()) { return nullptr; }
    return Object::SCast<Application>(
                GameWindow::GetInstance()->GetApplication());
    #endif
}

void Application::ResetDeltaTime()
{
    m_lastRenderTime = Time::GetNow();
}

bool Application::notify(QObject *receiver, QEvent *e)
{
    #ifdef BANG_EDITOR
    Screen *screen = Screen::GetInstance();
    if (screen && receiver == screen)
    {
        Input::GetInstance()->EnqueueEvent(e);
    }

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
        QKeyEvent *ev = Object::SCast<QKeyEvent>(e);
        m_lastKeyPressEvInfo.time = Time::GetNow();
        m_lastKeyPressEvInfo.key = ev->key();
    }

    #else // GAME

    Input *input = Input::GetInstance();
    if (input)
    {
        input->EnqueueEvent(e);
    }

    #endif

    return QApplication::notify(receiver, e);
}

bool Application::CurrentKeyReleaseIsAutoRepeat(const QKeyEvent *keyReleaseEvent)
{
    return m_lastKeyPressEvInfo.key == keyReleaseEvent->key() &&
           Time::GetNow() == m_lastKeyPressEvInfo.time;
}
