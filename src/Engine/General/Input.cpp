#include "Bang/Input.h"

#include "Bang/GL.h"
#include "Bang/Time.h"
#include "Bang/Window.h"
#include "Bang/Application.h"

USING_NAMESPACE_BANG

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
            kInfo.down = false; // Not down anymore, just pressed.
            m_keysDown.Remove(it->first);
        }

        if (kInfo.up) // After a frame where it was Up
        {
            m_keysUp.Remove(it->first);
            it = m_keyInfos.Remove(it);
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

        if (mbInfo.up)
        {
            it = m_mouseInfo.Remove(it);
        }
        else { ++it; }
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

    m_mouseCoords = Vector2i(ei.x, ei.y);

    if (m_lockMouseMovement)
    {
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
    m_isADoubleClick = false; // Reset double click

    m_mouseInfo.Add(mb, ButtonInfo(up, true, true));
    if (m_secsSinceLastMouseDown <= DoubleClickMaxSeconds &&
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
        // We use these ifs, because down & pressed & up can happen
        // in the same frame (this does happen sometimes)
        if (m_mouseInfo[mb].down)
        {
            m_mouseInfo.Add(mb, ButtonInfo(true, m_mouseInfo[mb].down,
                                           m_mouseInfo[mb].pressed));
        }
        else
        {
            m_mouseInfo.Add(mb, ButtonInfo(true, false, false));
        }
    }
}


void Input::ProcessKeyDownEventInfo(const EventInfo &ei)
{
    Key k = ei.key;
    if (m_keyInfos.ContainsKey(k))
    {
        m_keyInfos[k] = ButtonInfo();
    }
    m_keyInfos[k].down       = true;
    m_keyInfos[k].pressed    = true;
    m_keyInfos[k].autoRepeat = ei.autoRepeat;

    m_pressedKeys.PushBack(k);
    m_keysDown.PushBack(k);
    m_keysUp.Remove(k);
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

    m_pressedKeys.Remove(k);
    m_keysDown.Remove(k);
    m_keysUp.PushBack(k);
}

void Input::PeekEvent(const SDL_Event &event, const Window *window)
{
    if (!window->HasFocus()) { Reset(); return; }
    m_isMouseInside = window->HasFocus() && window->IsMouseOver();

    bool enqueue = false;
    EventInfo eventInfo;
    switch (event.type)
    {
        case SDL_KEYDOWN:
            eventInfo.type       = EventInfo::KeyDown;
            eventInfo.autoRepeat = event.key.repeat;
            eventInfo.key        = SCAST<Key>(event.key.keysym.sym);
            enqueue = true;
        break;

        case SDL_KEYUP:
            eventInfo.type = EventInfo::KeyUp;
            eventInfo.autoRepeat = event.key.repeat;
            eventInfo.key        = SCAST<Key>(event.key.keysym.sym);
            enqueue = true;
        break;

        case SDL_TEXTINPUT:
            m_inputText = String(event.text.text);
        break;
    }

    if (window->IsMouseOver())
    {
        switch(event.type)
        {
            case SDL_MOUSEBUTTONDOWN:
                eventInfo.type = EventInfo::MouseDown;
                eventInfo.mouseButton = SCAST<MouseButton>(event.button.button);
                enqueue = true;
            break;

            case SDL_MOUSEBUTTONUP:
                eventInfo.type = EventInfo::MouseUp;
                eventInfo.mouseButton = SCAST<MouseButton>(event.button.button);
                enqueue = true;
            break;

            case SDL_MOUSEWHEEL:
                eventInfo.type = EventInfo::Wheel;
                eventInfo.wheelDelta = float(event.wheel.y);
                enqueue = true;
            break;

            case SDL_MOUSEMOTION:
                eventInfo.type = EventInfo::MouseMove;
                eventInfo.x = event.motion.x;
                eventInfo.y = event.motion.y;
                enqueue = true;
            break;
        }
    }

    if (enqueue) { EnqueueEvent(eventInfo); }
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
    return Window::GetCurrent()->GetInput();
}

String KeyToString(Key k)
{
    return String( SDL_GetKeyName( SCAST<SDL_Keycode>(k) ) );
}

bool Input::GetKey(Key k)
{
    Input *inp = Input::GetInstance();
    return inp->m_keyInfos.ContainsKey(k) && inp->m_keyInfos[k].pressed;
}

bool Input::GetKeyUp(Key k)
{
    Input *inp = Input::GetInstance();
    return inp->m_keyInfos.ContainsKey(k) && inp-> m_keyInfos[k].up;
}

bool Input::GetKeyDown(Key k)
{
    Input *inp = Input::GetInstance();
    return  inp->m_keyInfos.ContainsKey(k) && inp->m_keyInfos[k].down &&
           !inp->m_keyInfos[k].autoRepeat;
}

bool Input::GetKeyDownRepeat(Key k)
{
    Input *inp = Input::GetInstance();
    return inp->m_keyInfos.ContainsKey(k) && inp->m_keyInfos[k].down;
}

const Array<Key> &Input::GetKeysUp()
{
    return Input::GetInstance()->m_keysUp;
}

const Array<Key> &Input::GetKeysDown()
{
    return Input::GetInstance()->m_keysDown;
}

const Array<Key>& Input::GetPressedKeys()
{
    return Input::GetInstance()->m_pressedKeys;
}

float Input::GetMouseWheel()
{
    Input *inp = Input::GetInstance();
    return inp->m_lastMouseWheelDelta;
}

Array<MouseButton> Input::GetMouseButtons()
{
    Array<MouseButton> mouseButtons;
    Input *inp = Input::GetInstance();
    for (auto it : inp->m_mouseInfo)
    {
        if (it.second.pressed) { mouseButtons.PushBack(it.first); }
    }
    return mouseButtons;
}
Array<MouseButton> Input::GetMouseButtonsUp()
{
    Array<MouseButton> mouseButtons;
    Input *inp = Input::GetInstance();
    for (auto it : inp->m_mouseInfo)
    {
        if (it.second.up) { mouseButtons.PushBack(it.first); }
    }
    return mouseButtons;
}
Array<MouseButton> Input::GetMouseButtonsDown()
{
    Array<MouseButton> mouseButtons;
    Input *inp = Input::GetInstance();
    for (auto it : inp->m_mouseInfo)
    {
        if (it.second.down) { mouseButtons.PushBack(it.first); }
    }
    return mouseButtons;
}


bool Input::GetMouseButton(MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseInfo.ContainsKey(mb) && inp->m_mouseInfo[mb].pressed;
}

bool Input::GetMouseButtonUp(MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseInfo.ContainsKey(mb) && inp->m_mouseInfo[mb].up;
}

bool Input::GetMouseButtonDown(MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseInfo.ContainsKey(mb) && inp->m_mouseInfo[mb].down;
}

bool Input::GetMouseButtonDoubleClick(MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return Input::GetMouseButtonUp(mb) && inp->m_isADoubleClick;
}

bool Input::IsMouseInsideScreen()
{
    return Input::GetInstance()->m_isMouseInside;
}

float Input::GetMouseAxisX()
{
    return Input::GetMouseDeltaX() / GL::GetViewportSize().x;
}

float Input::GetMouseAxisY()
{
    return Input::GetMouseDeltaY() / GL::GetViewportSize().y;
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

Vector2i Input::GetMouseCoords()
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseCoords;
}

Vector2 Input::GetMouseCoordsNDC()
{
    Vector2f coordsNDC = Vector2f(Input::GetMouseCoords()) *
                                  GL::GetViewportPixelSize() * 2.0f - 1.0f;
    return coordsNDC * Vector2f(1, -1);
}

Vector2i Input::GetPreviousMouseCoords()
{
    Input *inp = Input::GetInstance();
    return inp->m_lastMouseCoords;
}

void Input::StartTextInput()
{
    SDL_StartTextInput();
}

String Input::PollInputText()
{
    Input *input = Input::GetInstance();
    String res = input->m_inputText;
    input->m_inputText = "";
    return res;
}

void Input::StopTextInput()
{
    SDL_StopTextInput();
}

void Input::Reset()
{
    m_isADoubleClick = m_lockMouseMovement = m_isMouseInside = false;
    m_framesMouseStopped = 0;
    m_lastMouseWheelDelta = m_secsSinceLastMouseDown = 0.0f;
    m_mouseCoords = m_lastMouseCoords = Vector2i(-1);
    m_inputText = "";

    m_keysUp.Clear();
    m_keysDown.Clear();
    m_pressedKeys.Clear();

    m_keyInfos.Clear();
    m_mouseInfo.Clear();
    m_eventInfoQueue.Clear();
}
