#include "Input.h"

#include "Time.h"
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
    // KEYS
    for (auto it = m_keyInfos.Begin(); it != m_keyInfos.End(); ++it)
    {
        ButtonInfo &kInfo = it->second;
        if (kInfo.down)
        {
            kInfo.down = false; //Not down anymore, just pressed.
        }

        if (kInfo.up) //After a frame where it was Up
        {
            m_keyInfos.Remove(it);
        }
    }
    //

    // MOUSE
    for (auto it = m_mouseInfo.Begin(); it != m_mouseInfo.End(); ++it)
    {
        ButtonInfo &mbInfo = it->second;
        if (mbInfo.down)
        {
            mbInfo.down = false; // Not down anymore, just pressed.
        }

        if (mbInfo.up)
        {
            m_mouseInfo.Remove(it);
        }

        m_isADoubleClick = false; // Reset double click
    }
    //

    m_lastMouseWheelDelta = 0.0f;

    // MOUSE LOCK
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

        Screen *screen = Screen::GetInstance();
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
        ProcessKeyPressEventInfo(ei);
    }
    else if (ei.m_eventType == QEvent::KeyRelease)
    {
        ProcessKeyReleasedEventInfo(ei);
    }
}

void Input::ProcessMouseWheelEventInfo(const EventInfo &ei)
{
    m_lastMouseWheelDelta = float(ei.m_wheelDelta);
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
    MouseButton mb = ei.m_mouseButton;
    bool up = false;
    if (m_mouseInfo.ContainsKey(mb))
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
    MouseButton mb = ei.m_mouseButton;
    if (m_mouseInfo.ContainsKey(mb))
    {
        // Only if it was pressed before
        // We must respect the down and pressed, just in case they happen
        // in the same frame (this does happen sometimes)
        m_mouseInfo[mb] = ButtonInfo(true, m_mouseInfo[mb].down, m_mouseInfo[mb].pressed);
    }
}


void Input::ProcessKeyPressEventInfo(const EventInfo &ei)
{
    if (ei.m_autoRepeat) return;

    Key k = ei.m_key;
    if (m_keyInfos.ContainsKey(k))
    {
        m_keyInfos[k] = ButtonInfo();
    }
    m_keyInfos[k].down    = true;
    m_keyInfos[k].pressed = true;

}

void Input::ProcessKeyReleasedEventInfo(const EventInfo &ei)
{
    if (ei.m_autoRepeat) return;

    Key k = ei.m_key;
    if (m_keyInfos.ContainsKey(k))
    {
        m_keyInfos[k] = ButtonInfo();
    }
    m_keyInfos[k].up = true;
}

void Input::EnqueueEvent(QEvent *event)
{
    EventInfo ei(event);
    m_eventInfoQueue.PushBack(ei);
}

void Input::ProcessEnqueuedEvents()
{
    for (const EventInfo &ei : m_eventInfoQueue)
    {
        ProcessEventInfo(ei);
    }
    m_eventInfoQueue.Clear();
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
    return inp->m_keyInfos.ContainsKey(k) && inp->m_keyInfos[k].pressed;
}

bool Input::GetKeyUp(Input::Key k)
{
    Input *inp = Input::GetInstance();
    return inp->m_keyInfos.ContainsKey(k) && inp-> m_keyInfos[k].up;
}

bool Input::GetKeyDown(Input::Key k)
{
    Input *inp = Input::GetInstance();
    return inp->m_keyInfos.ContainsKey(k) && inp->m_keyInfos[k].down;
}

float Input::GetMouseWheel()
{
    Input *inp = Input::GetInstance();
    return inp->m_lastMouseWheelDelta;
}


bool Input::GetMouseButton(Input::MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseInfo.ContainsKey(mb) && inp->m_mouseInfo[mb].pressed;
}

bool Input::GetMouseButtonUp(Input::MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseInfo.ContainsKey(mb) && inp->m_mouseInfo[mb].up;
}

bool Input::GetMouseButtonDown(Input::MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseInfo.ContainsKey(mb) && inp->m_mouseInfo[mb].down;
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

Vector2 Input::GetPreviousMouseCoords()
{
    Input *inp = Input::GetInstance();
    return inp->m_lastMouseCoords;
}


Input::EventInfo::EventInfo(const QEvent *e)
{
    m_eventType = e->type();
    if (m_eventType == QEvent::KeyPress ||
            m_eventType == QEvent::KeyRelease)
    {
        const QKeyEvent *ke = static_cast<const QKeyEvent*>(e);
        int k = ke->key();
        m_autoRepeat = ke->isAutoRepeat();
        m_key = static_cast<Input::Key>(k);
    }
    else if (m_eventType == QEvent::MouseButtonPress ||
             m_eventType == QEvent::MouseButtonRelease ||
             m_eventType == QEvent::MouseMove)
    {
        const QMouseEvent *me = static_cast<const QMouseEvent*>(e);
        Qt::MouseButton mb = me->button();
        m_mouseButton = static_cast<Input::MouseButton>(mb);
        m_x = me->pos().x();
        m_y = me->pos().y();
    }
    else if (m_eventType == QEvent::Wheel)
    {
        const QWheelEvent *we = static_cast<const QWheelEvent*>(e);
        m_wheelDelta = we->angleDelta().y();
    }
}
