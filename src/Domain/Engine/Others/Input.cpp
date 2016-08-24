#include "Input.h"

#include "Screen.h"
#include "Application.h"
#include "SingletonManager.h"

#ifdef BANG_EDITOR
#include "EditorWindow.h"
#else
#include "GameWindow.h"
#endif

Input::Input()
{
}

void Input::InitFromMainBinary() // Called from Screen
{
    SingletonManager::GetInstance()->SetInputSingleton(new Input());
}

void Input::OnFrameFinished()
{
    Debug_Log("Input::OnFrameFinished");
    // KEYS
    // Debug_Log("BEFORE: " << m_keyInfos);
    for (auto it = m_keyInfos.begin(); it != m_keyInfos.end(); ++it)
    {
        ButtonInfo &kInfo = it->second;
        if (kInfo.down)
        {
            kInfo.down = false; //Not down anymore, just pressed.
        }

        if (kInfo.up) //After a frame where it was Up
        {
            m_keyInfos.erase(it);
        }
    }
    // Debug_Log("AFTER: " << m_keyInfos);
    //

    // MOUSE
    Debug_Log("BEFORE: " << m_mouseInfo);
    for (auto it = m_mouseInfo.begin(); it != m_mouseInfo.end(); ++it)
    {
        ButtonInfo &mbInfo = it->second;
        if (mbInfo.down)
        {
            mbInfo.down = false; // Not down anymore, just pressed.
        }

        if (mbInfo.up)
        {
            m_mouseInfo.erase(it);
        }

        m_isADoubleClick = false; // Reset double click
    }
    Debug_Log("AFTER: " << m_mouseInfo);
    //

    // MOUSE LOCK
    m_lastMouseWheelDelta = 0.0f;
    if (!m_lockMouseMovement)
    {
        m_lastMouseCoords = m_mouseCoords;
    }
    else if (m_framesMouseStopped > 3)
    {
        //For the case of mouse movement locking, this
        //avoids a bug of axisX/Y getting stuck when mouse
        //does: lock/moving/stom_moving => axisX/Y think mouse is still moving
        m_lastMouseCoords = m_mouseCoords;
    }
    ++m_framesMouseStopped;
    //

    m_secsSinceLastMouseDown += Time::GetDeltaTime();

    HandleMouseWrapping();
}

void Input::HandleMouseWrapping()
{
    if (m_mouseWrapping)
    {
        #ifdef BANG_EDITOR
        EditorWindow *w = EditorWindow::GetInstance();
        #else
        GameWindow *w = GameWindow::GetInstance();
        #endif

        QCursor cursor = w->GetMainWindow()->cursor();

        Screen *screen = w->screen;
        int cw = screen->GetWidth();
        int ch = screen->GetHeight();

        bool wrapped = false;
        if (m_mouseCoords.x >= cw)
        {
            cursor.setPos(screen->mapToGlobal(QPoint(0, m_mouseCoords.y)));
            wrapped = true;
        }
        else if (m_mouseCoords.x < 0)
        {
            cursor.setPos(screen->mapToGlobal(QPoint(cw, m_mouseCoords.y)));
            wrapped = true;
        }

        if (m_mouseCoords.y >= ch)
        {
            cursor.setPos(screen->mapToGlobal(QPoint(m_mouseCoords.x, 0)));
            wrapped = true;
        }
        else if (m_mouseCoords.y < 0)
        {
            cursor.setPos(screen->mapToGlobal(QPoint(m_mouseCoords.x, ch)));
            wrapped = true;
        }

        if (wrapped)
        {
            QPoint newCoords = screen->mapFromGlobal(cursor.pos());
            m_mouseCoords = Vector2(newCoords.x(), newCoords.y());
            m_lastMouseCoords = m_mouseCoords;
        }
    }
}

void Input::ProcessEventInfo(const EventInfo &ei)
{
    if (ei.m_eventType == QEvent::Wheel)
    {
        ProcessMouseWheelEventInfo(ei);
    }
    else if (ei.m_eventType == QEvent::MouseMove)
    {
        ProcessMouseMoveEventInfo(ei);
    }
    else if (ei.m_eventType == QEvent::MouseButtonPress)
    {
        ProcessMousePressEventInfo(ei);
    }
    else if (ei.m_eventType == QEvent::MouseButtonRelease)
    {
        ProcessMouseReleaseEventInfo(ei);
    }
    else if (ei.m_eventType == QEvent::KeyPress)
    {
        Debug_Log("Processing keyPress");
        ProcessKeyPressEventInfo(ei);
    }
    else if (ei.m_eventType == QEvent::KeyRelease)
    {
        Debug_Log("Processing keyRelease");
        ProcessKeyReleasedEventInfo(ei);
    }
}

void Input::ProcessMouseWheelEventInfo(const EventInfo &ei)
{
    m_lastMouseWheelDelta = float(ei.m_wheelDelta) / (360.0f);
}

void Input::ProcessMouseMoveEventInfo(const EventInfo &ei)
{
    m_framesMouseStopped = 0;

    //Used to ignore QCursor::setPos call to mouseMove event
    static bool fakeMoveEvent = false;
    if (fakeMoveEvent)
    {
        fakeMoveEvent = false;
        return;
    }

    m_mouseCoords = Vector2(ei.m_x, ei.m_y);

    if (m_lockMouseMovement)
    {
        QPoint glob = QPoint(m_lastMouseCoords.x,
                             m_lastMouseCoords.y);
        glob = Screen::GetInstance()->mapToGlobal(glob);

        fakeMoveEvent = true;
        QCursor::setPos(glob);
    }
}

void Input::ProcessMousePressEventInfo(const EventInfo &ei)
{
    Debug_Log("ProcessMousePressEventInfo");
    MouseButton mb = ei.m_mouseButton;
    bool up = false;
    if (m_mouseInfo.find(mb) != m_mouseInfo.end())
    {
        up = m_mouseInfo[mb].up;
    }

    m_mouseInfo[mb] = ButtonInfo(up, true, true);
    if (m_secsSinceLastMouseDown <= c_doubleClickMaxSeconds &&
        m_secsSinceLastMouseDown != 0)
        // Reject clicks in the same frame as double-clicking (secs... != 0)
        // To avoid a delayed MouseEvent bug
    {
        m_isADoubleClick = true;
    }
    m_secsSinceLastMouseDown = 0; // Restart time since last click counter
}

void Input::ProcessMouseReleaseEventInfo(const EventInfo &ei)
{
    Debug_Log("ProcessMouseReleaseEventInfo");
    MouseButton mb = ei.m_mouseButton;
    if (m_mouseInfo.find(mb) != m_mouseInfo.end())
    {
        // Only if it was pressed before
        // We must respect the down and pressed, just in case they happen
        // in the same frame (this does happen sometimes)
        m_mouseInfo[mb] = ButtonInfo(true, m_mouseInfo[mb].down, m_mouseInfo[mb].pressed);
    }
}


void Input::ProcessKeyPressEventInfo(const EventInfo &ei)
{
    Debug_Log("ProcessKeyPressEventInfo");
    Key k = ei.m_key;
    if (m_keyInfos.find(k) == m_keyInfos.end())
    {
        m_keyInfos[k] = ButtonInfo();
    }
    m_keyInfos[k].down    = true;
    m_keyInfos[k].pressed = true;

}

void Input::ProcessKeyReleasedEventInfo(const EventInfo &ei)
{
    Debug_Log("ProcessKeyReleasedEventInfo");
    Key k = ei.m_key;
    if (m_keyInfos.find(k) == m_keyInfos.end())
    {
        m_keyInfos[k] = ButtonInfo();
    }
    m_keyInfos[k].up = true;
}

void Input::EnqueueEvent(QEvent *event)
{
    EventInfo ei(event);
    m_eventInfoQueue.push_back(ei);
}

void Input::ProcessEnqueuedEvents()
{
    Debug_Log("ProcessEnqueuedEvents before");
    Debug_Log("Enqueued events: " << m_eventInfoQueue.size());
    for (const EventInfo &ei : m_eventInfoQueue)
    {
        ProcessEventInfo(ei);
    }
    m_eventInfoQueue.clear();
    Debug_Log("ProcessEnqueuedEvents after");
}

Input *Input::GetInstance()
{
    return SingletonManager::GetInstance()->GetInputSingleton();
}

bool Input::GetMouseWrapping()
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseWrapping;
}

void Input::SetMouseWrapping(bool mouseWrapping)
{
    Input *inp = Input::GetInstance();
    inp->m_mouseWrapping = mouseWrapping;
}


bool Input::GetKey(Input::Key k)
{
    Input *inp = Input::GetInstance();
    return inp->m_keyInfos.find(k) != inp->m_keyInfos.end() &&
           inp->m_keyInfos[k].pressed;
}

bool Input::GetKeyUp(Input::Key k)
{
    Input *inp = Input::GetInstance();
    return inp->m_keyInfos.find(k) != inp->m_keyInfos.end() &&
           inp-> m_keyInfos[k].up;
}

bool Input::GetKeyDown(Input::Key k)
{
    Input *inp = Input::GetInstance();
    return inp->m_keyInfos.find(k) != inp->m_keyInfos.end() &&
           inp->m_keyInfos[k].down;
}

float Input::GetMouseWheel()
{
    Input *inp = Input::GetInstance();
    return inp->m_lastMouseWheelDelta;
}


bool Input::GetMouseButton(Input::MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseInfo.find(mb) != inp->m_mouseInfo.end() &&
           inp->m_mouseInfo[mb].pressed;
}

bool Input::GetMouseButtonUp(Input::MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseInfo.find(mb) != inp->m_mouseInfo.end() &&
           inp->m_mouseInfo[mb].up;
}

bool Input::GetMouseButtonDown(Input::MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseInfo.find(mb) != inp->m_mouseInfo.end() &&
           inp->m_mouseInfo[mb].down;
}

bool Input::GetMouseButtonDoubleClick(Input::MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return Input::GetMouseButtonDown(mb) && inp->m_isADoubleClick;
}

float Input::GetMouseAxisX()
{
    return Input::GetMouseDeltaX() / Screen::GetWidth();
}

float Input::GetMouseAxisY()
{
    return Input::GetMouseDeltaY() / Screen::GetHeight();
}

Vector2 Input::GetMouseAxis()
{
    return Vector2(Input::GetMouseAxisX(), Input::GetMouseAxisY());
}

float Input::GetMouseDeltaX()
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseCoords.x - inp->m_lastMouseCoords.x;
}

float Input::GetMouseDeltaY()
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseCoords.y - inp->m_lastMouseCoords.y;
}

Vector2 Input::GetMouseDelta()
{
    return Vector2(Input::GetMouseDeltaX(), Input::GetMouseDeltaY());
}

void Input::LockMouseMovement(bool lock)
{
    Input *inp = Input::GetInstance();
    inp->m_lockMouseMovement = lock;
}

bool Input::IsLockMouseMovement()
{
    Input *inp = Input::GetInstance();
    return inp->m_lockMouseMovement;
}

Vector2 Input::GetMouseCoords()
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseCoords;
}

