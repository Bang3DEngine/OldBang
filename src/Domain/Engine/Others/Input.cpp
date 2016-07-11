#include "Input.h"

#include "SingletonManager.h"
#include "WindowMain.h"
#include "Canvas.h"

Input::Input()
{
}

void Input::InitFromMainBinary() // Called from Canvas
{
    SingletonManager::GetInstance()->SetInputSingleton(new Input());
}

void Input::OnNewFrame()
{
    for (auto it = m_keyInfos.begin(); it != m_keyInfos.end(); ++it)
    {
        ButtonInfo &kInfo = it->second;
        if (kInfo.up) //After a frame where it was Up
        {
            m_keyInfos.erase(it);
        }
        else if (kInfo.down)
        {
            kInfo.down = false; //Not down anymore, just pressed.
        }
    }

    for (auto it = m_mouseInfo.begin(); it != m_mouseInfo.end(); ++it)
    {
        ButtonInfo &mbInfo = it->second;
        if (mbInfo.up) //After a frame where it was Up
        {
            m_mouseInfo.erase(it);
        }
        else if (mbInfo.down)
        {
            mbInfo.down = false; // Not down anymore, just pressed.
        }

        m_isADoubleClick = false; // Reset double click
    }

    m_lastMouseWheelDelta = 0.0f;
    if (!m_lockMouseMovement)
    {
        m_lastMouseCoords = m_mouseCoords;
    }
    else if (m_framesMouseStopped > 3)
    {
        //For the case of mouse movement locking, this
        //avoids a bug of axisX/Y getting stuck when mouse
        //does: lock/moving/stop_moving => axisX/Y think mouse is still moving
        m_lastMouseCoords = m_mouseCoords;
    }
    ++m_framesMouseStopped;

    m_secsSinceLastMouseDown += Time::GetDeltaTime();

    HandleMouseWrapping();
}

void Input::HandleMouseWrapping()
{
    if(m_mouseWrapping)
    {
        QCursor cursor = WindowMain::GetMainWindow()->cursor();
        WindowMain *w = WindowMain::GetInstance();
        Canvas *canvas = w->canvas;
        int cw = canvas->GetWidth();
        int ch = canvas->GetHeight();

        bool wrapped = false;
        if(m_mouseCoords.x >= cw)
        {
            cursor.setPos(canvas->mapToGlobal(QPoint(0, m_mouseCoords.y)));
            wrapped = true;
        }
        else if(m_mouseCoords.x < 0)
        {
            cursor.setPos(canvas->mapToGlobal(QPoint(cw, m_mouseCoords.y)));
            wrapped = true;
        }

        if(m_mouseCoords.y >= ch)
        {
            cursor.setPos(canvas->mapToGlobal(QPoint(m_mouseCoords.x, 0)));
            wrapped = true;
        }
        else if(m_mouseCoords.y < 0)
        {
            cursor.setPos(canvas->mapToGlobal(QPoint(m_mouseCoords.x, ch)));
            wrapped = true;
        }

        if(wrapped)
        {
            QPoint newCoords = canvas->mapFromGlobal(cursor.pos());
            m_mouseCoords = glm::vec2(newCoords.x(), newCoords.y());
            m_lastMouseCoords = m_mouseCoords;
        }
    }
}

void Input::HandleInputMouseWheel(QWheelEvent *event)
{
    m_lastMouseWheelDelta = float(event->delta()) / (360.0f);
}

void Input::HandleInputMouseMove(QMouseEvent *event)
{
    m_framesMouseStopped = 0;

    //Used to ignore QCursor::setPos call to mouseMove event
    static bool fakeMoveEvent = false;
    if(fakeMoveEvent)
    {
        fakeMoveEvent = false;
        return;
    }

    m_mouseCoords = glm::vec2(event->x(), event->y());

    if(m_lockMouseMovement)
    {
        QPoint glob = QPoint(m_lastMouseCoords.x,
                             m_lastMouseCoords.y);
        glob = Canvas::GetInstance()->mapToGlobal(glob);

        fakeMoveEvent = true;
        QCursor::setPos(glob);
    }
}

void Input::HandleInputMousePress(QMouseEvent *event)
{
    MouseButton mb = static_cast<MouseButton>(event->button());
    if(m_mouseInfo.find(mb) == m_mouseInfo.end())
    {
        //Only if it was not down/pressed before
        m_mouseInfo[mb] = ButtonInfo(false, true, true);

        if(m_secsSinceLastMouseDown <= c_doubleClickMaxSeconds)
        {
            m_isADoubleClick = true;
        }
        m_secsSinceLastMouseDown = 0; // Restart time since last click counter
    }
}

void Input::HandleInputMouseRelease(QMouseEvent *event)
{
    MouseButton mb = static_cast<MouseButton>(event->button());
    if(m_mouseInfo.find(mb) != m_mouseInfo.end() &&
       m_mouseInfo[mb].pressed)
    {
        //Only if it was pressed before
        m_mouseInfo[mb] = ButtonInfo(true, false, false);
    }
}


void Input::HandleInputKeyPress(QKeyEvent *event)
{
    //Only capture first press, not repeated ones
    if(event->isAutoRepeat()) return;

    Key k = static_cast<Key>(event->key());
    if(m_keyInfos.find(k) == m_keyInfos.end())
    {   //Only if it was not down/pressed before
        m_keyInfos[k] = ButtonInfo(false, true, true);
    }
}

void Input::HandleInputKeyReleased(QKeyEvent *event)
{
    //Only capture actual release, not repeated ones
    if(event->isAutoRepeat()) return;

    Key k = static_cast<Key>(event->key());
    if(m_keyInfos.find(k) != m_keyInfos.end() &&
       m_keyInfos[k].pressed)
    {   //Only if it was pressed before
        m_keyInfos[k] = ButtonInfo(true, false, false);
    }
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
    return Input::GetMouseDeltaX() / Canvas::GetWidth();
}

float Input::GetMouseAxisY()
{
    return Input::GetMouseDeltaY() / Canvas::GetHeight();
}

glm::vec2 Input::GetMouseAxis()
{
    return glm::vec2(Input::GetMouseAxisX(), Input::GetMouseAxisY());
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

glm::vec2 Input::GetMouseDelta()
{
    return glm::vec2(Input::GetMouseDeltaX(), Input::GetMouseDeltaY());
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

glm::vec2 Input::GetMouseCoords()
{
    Input *inp = Input::GetInstance();
    return inp->m_mouseCoords;
}
