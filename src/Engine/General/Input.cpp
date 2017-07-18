#include "Bang/Input.h"

#include "Bang/Time.h"
#include "Bang/Object.h"
#include "Bang/Window.h"
#include "Bang/Screen.h"
#include "Bang/Application.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::OnFrameFinished()
{
    // KEYS
    for (auto it = m_keyInfos.Begin(); it != m_keyInfos.End();)
    {
        ButtonInfo &kInfo = it->second;
        if (kInfo.down)
        {
            kInfo.down = false; //Not down anymore, just pressed.
        }

        if (kInfo.up) //After a frame where it was Up
        {
            m_keyInfos.Remove(it++);
        }
        else { ++it; }
    }
    //

    // MOUSE
    for (auto it = m_mouseInfo.Begin(); it != m_mouseInfo.End();)
    {
        ButtonInfo &mbInfo = it->second;
        if (mbInfo.down)
        {
            mbInfo.down = false; // Not down anymore, just pressed.
        }

        if (mbInfo.up) { m_mouseInfo.Remove(it++); }
        else { ++it; }

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
}

void Input::ProcessEventInfo(const EventInfo &ei)
{
    if (ei.type == EventInfo::Type::Wheel)
    {
        ProcessMouseWheelEventInfo(ei);
    }
    else if (ei.type == EventInfo::Type::MouseMove)
    {
        ProcessMouseMoveEventInfo(ei);
    }
    else if (ei.type == EventInfo::Type::MouseDown)
    {
        ProcessMouseDownEventInfo(ei);
    }
    else if (ei.type == EventInfo::Type::MouseUp)
    {
        ProcessMouseUpEventInfo(ei);
    }
    else if (ei.type == EventInfo::Type::KeyDown)
    {
        ProcessKeyDownEventInfo(ei);
    }
    else if (ei.type == EventInfo::Type::KeyUp)
    {
        ProcessKeyUpEventInfo(ei);
    }
}

void Input::ProcessMouseWheelEventInfo(const EventInfo &ei)
{
    m_lastMouseWheelDelta = float(ei.wheelDelta);
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

    m_mouseCoords = Vector2(ei.x, ei.y);

    if (m_lockMouseMovement)
    {
        /*
        QPoint glob = QPoint(m_lastMouseCoords.x,
                             m_lastMouseCoords.y);
        glob = Screen::GetInstance()->mapToGlobal(glob);

        fakeMoveEvent = true;
        QCursor::setPos(glob);
        */
    }
}

void Input::ProcessMouseDownEventInfo(const EventInfo &ei)
{
    MouseButton mb = ei.mouseButton;
    bool up = false;
    if (m_mouseInfo.ContainsKey(mb))
    {
        up = m_mouseInfo.Get(mb).up;
    }

    m_mouseInfo.Set(mb, ButtonInfo(up, true, true));
    if (m_secsSinceLastMouseDown <= c_doubleClickMaxSeconds &&
        m_secsSinceLastMouseDown != 0)
        // Reject clicks in the same frame as double-clicking (secs... != 0)
        // To avoid a delayed MouseEvent bug
    {
        m_isADoubleClick = true;
    }
    m_secsSinceLastMouseDown = 0; // Restart time since last click counter
}

void Input::ProcessMouseUpEventInfo(const EventInfo &ei)
{
    MouseButton mb = ei.mouseButton;
    if (m_mouseInfo.ContainsKey(mb))
    {
        // Only if it was pressed before
        // We must respect the down and pressed, just in case they happen
        // in the same frame (this does happen sometimes)
        m_mouseInfo.Set(mb, ButtonInfo(true, m_mouseInfo[mb].down,
                                       m_mouseInfo[mb].pressed));
    }
}


void Input::ProcessKeyDownEventInfo(const EventInfo &ei)
{
    if (ei.autoRepeat) return;

    Key k = ei.key;
    if (m_keyInfos.ContainsKey(k))
    {
        m_keyInfos[k] = ButtonInfo();
    }
    m_keyInfos[k].down    = true;
    m_keyInfos[k].pressed = true;

}

void Input::ProcessKeyUpEventInfo(const EventInfo &ei)
{
    if (ei.autoRepeat) return;

    Key k = ei.key;
    if (m_keyInfos.ContainsKey(k))
    {
        m_keyInfos[k] = ButtonInfo();
    }
    m_keyInfos[k].up = true;
}

void Input::PeekEvent(const SDL_Event &event)
{
    bool enqueue = true;
    EventInfo eventInfo;
    switch (event.type)
    {
        case SDL_KEYDOWN:
            eventInfo.type = EventInfo::KeyDown;
            eventInfo.autoRepeat = event.key.repeat;
            eventInfo.key        = SCAST<Key>(event.key.keysym.sym);
        break;

        case SDL_KEYUP:
            eventInfo.type = EventInfo::KeyUp;
            eventInfo.autoRepeat = event.key.repeat;
            eventInfo.key        = SCAST<Key>(event.key.keysym.sym);
        break;

        case SDL_MOUSEBUTTONDOWN:
            eventInfo.type = EventInfo::MouseDown;
            eventInfo.mouseButton = SCAST<MouseButton>(event.button.button);
        break;

        case SDL_MOUSEBUTTONUP:
            eventInfo.type = EventInfo::MouseUp;
            eventInfo.mouseButton = SCAST<MouseButton>(event.button.button);
        break;

        case SDL_MOUSEWHEEL:
            eventInfo.type = EventInfo::Wheel;
            eventInfo.wheelDelta = float(event.wheel.y);
        break;

        case SDL_MOUSEMOTION:
            eventInfo.type = EventInfo::MouseMove;
            eventInfo.x = event.motion.x;
            eventInfo.y = event.motion.y;
        break;

        default:
            enqueue = false;
    }

    if (enqueue)
    {
        EnqueueEvent(eventInfo);
    }
}

void Input::EnqueueEvent(const EventInfo &eventInfo)
{
    m_eventInfoQueue.PushBack(eventInfo);
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
    return Application::GetInstance()->GetInput();
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

Vector2 Input::GetMouseCoordsNDC()
{
    return Input::GetMouseCoords() * Screen::GetPixelClipSize() * 2.0f - 1.0f;
}

Vector2 Input::GetPreviousMouseCoords()
{
    Input *inp = Input::GetInstance();
    return inp->m_lastMouseCoords;
}
