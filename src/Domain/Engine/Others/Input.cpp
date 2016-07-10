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
    for (auto it = keyInfos.begin(); it != keyInfos.end(); ++it)
    {
        ButtonInfo &kInfo = it->second;
        if (kInfo.up) //After a frame where it was Up
        {
            keyInfos.erase(it);
        }
        else if (kInfo.down)
        {
            kInfo.down = false; //Not down anymore, just pressed.
        }
    }

    for (auto it = mouseInfo.begin(); it != mouseInfo.end(); ++it)
    {
        ButtonInfo &mbInfo = it->second;
        if (mbInfo.up) //After a frame where it was Up
        {
            mouseInfo.erase(it);
        }
        else if (mbInfo.down)
        {
            mbInfo.down = false; // Not down anymore, just pressed.
        }

        isADoubleClick = false; // Reset double click
    }

    lastMouseWheelDelta = 0.0f;
    if (!lockMouseMovement)
    {
        lastMouseCoords = mouseCoords;
    }
    else if (framesMouseStopped > 3)
    {
        //For the case of mouse movement locking, this
        //avoids a bug of axisX/Y getting stuck when mouse
        //does: lock/moving/stop_moving => axisX/Y think mouse is still moving
        lastMouseCoords = mouseCoords;
    }
    ++framesMouseStopped;

    secsSinceLastMouseDown += Time::GetDeltaTime();

    HandleMouseWrapping();
}

void Input::HandleMouseWrapping()
{
    if(mouseWrapping)
    {
        QCursor cursor = WindowMain::GetMainWindow()->cursor();
        WindowMain *w = WindowMain::GetInstance();
        Canvas *canvas = w->canvas;
        int cw = canvas->GetWidth();
        int ch = canvas->GetHeight();

        bool wrapped = false;
        if(mouseCoords.x >= cw)
        {
            cursor.setPos(canvas->mapToGlobal(QPoint(0, mouseCoords.y)));
            wrapped = true;
        }
        else if(mouseCoords.x < 0)
        {
            cursor.setPos(canvas->mapToGlobal(QPoint(cw, mouseCoords.y)));
            wrapped = true;
        }

        if(mouseCoords.y >= ch)
        {
            cursor.setPos(canvas->mapToGlobal(QPoint(mouseCoords.x, 0)));
            wrapped = true;
        }
        else if(mouseCoords.y < 0)
        {
            cursor.setPos(canvas->mapToGlobal(QPoint(mouseCoords.x, ch)));
            wrapped = true;
        }

        if(wrapped)
        {
            QPoint newCoords = canvas->mapFromGlobal(cursor.pos());
            mouseCoords = glm::vec2(newCoords.x(), newCoords.y());
            lastMouseCoords = mouseCoords;
        }
    }
}

void Input::HandleInputMouseWheel(QWheelEvent *event)
{
    lastMouseWheelDelta = float(event->delta()) / (360.0f);
}

void Input::HandleInputMouseMove(QMouseEvent *event)
{
    framesMouseStopped = 0;

    //Used to ignore QCursor::setPos call to mouseMove event
    static bool fakeMoveEvent = false;
    if(fakeMoveEvent)
    {
        fakeMoveEvent = false;
        return;
    }

    mouseCoords = glm::vec2(event->x(), event->y());

    if(lockMouseMovement)
    {
        QPoint glob = QPoint(lastMouseCoords.x,
                             lastMouseCoords.y);
        glob = Canvas::GetInstance()->mapToGlobal(glob);

        fakeMoveEvent = true;
        QCursor::setPos(glob);
    }
}

void Input::HandleInputMousePress(QMouseEvent *event)
{
    MouseButton mb = static_cast<MouseButton>(event->button());
    if(mouseInfo.find(mb) == mouseInfo.end())
    {
        //Only if it was not down/pressed before
        mouseInfo[mb] = ButtonInfo(false, true, true);

        if(secsSinceLastMouseDown <= doubleClickMaxSeconds)
        {
            isADoubleClick = true;
        }
        secsSinceLastMouseDown = 0; // Restart time since last click counter
    }
}

void Input::HandleInputMouseRelease(QMouseEvent *event)
{
    MouseButton mb = static_cast<MouseButton>(event->button());
    if(mouseInfo.find(mb) != mouseInfo.end() &&
       mouseInfo[mb].pressed)
    {
        //Only if it was pressed before
        mouseInfo[mb] = ButtonInfo(true, false, false);
    }
}


void Input::HandleInputKeyPress(QKeyEvent *event)
{
    //Only capture first press, not repeated ones
    if(event->isAutoRepeat()) return;

    Key k = static_cast<Key>(event->key());
    if(keyInfos.find(k) == keyInfos.end())
    {   //Only if it was not down/pressed before
        keyInfos[k] = ButtonInfo(false, true, true);
    }
}

void Input::HandleInputKeyReleased(QKeyEvent *event)
{
    //Only capture actual release, not repeated ones
    if(event->isAutoRepeat()) return;

    Key k = static_cast<Key>(event->key());
    if(keyInfos.find(k) != keyInfos.end() &&
       keyInfos[k].pressed)
    {   //Only if it was pressed before
        keyInfos[k] = ButtonInfo(true, false, false);
    }
}

Input *Input::GetInstance()
{
    return SingletonManager::GetInstance()->GetInputSingleton();
}

bool Input::GetMouseWrapping()
{
    Input *inp = Input::GetInstance();
    return inp->mouseWrapping;
}

void Input::SetMouseWrapping(bool mouseWrapping)
{
    Input *inp = Input::GetInstance();
    inp->mouseWrapping = mouseWrapping;
}


bool Input::GetKey(Input::Key k)
{
    Input *inp = Input::GetInstance();
    return inp->keyInfos.find(k) != inp->keyInfos.end() &&
           inp->keyInfos[k].pressed;
}

bool Input::GetKeyUp(Input::Key k)
{
    Input *inp = Input::GetInstance();
    return inp->keyInfos.find(k) != inp->keyInfos.end() &&
           inp-> keyInfos[k].up;
}

bool Input::GetKeyDown(Input::Key k)
{
    Input *inp = Input::GetInstance();
    return inp->keyInfos.find(k) != inp->keyInfos.end() &&
           inp->keyInfos[k].down;
}

float Input::GetMouseWheel()
{
    Input *inp = Input::GetInstance();
    return inp->lastMouseWheelDelta;
}


bool Input::GetMouseButton(Input::MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return inp->mouseInfo.find(mb) != inp->mouseInfo.end() &&
           inp->mouseInfo[mb].pressed;
}

bool Input::GetMouseButtonUp(Input::MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return inp->mouseInfo.find(mb) != inp->mouseInfo.end() &&
           inp->mouseInfo[mb].up;
}

bool Input::GetMouseButtonDown(Input::MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return inp->mouseInfo.find(mb) != inp->mouseInfo.end() &&
           inp->mouseInfo[mb].down;
}

bool Input::GetMouseButtonDoubleClick(Input::MouseButton mb)
{
    Input *inp = Input::GetInstance();
    return Input::GetMouseButtonDown(mb) && inp->isADoubleClick;
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
    return inp->mouseCoords.x - inp->lastMouseCoords.x;
}

float Input::GetMouseDeltaY()
{
    Input *inp = Input::GetInstance();
    return inp->mouseCoords.y - inp->lastMouseCoords.y;
}

glm::vec2 Input::GetMouseDelta()
{
    return glm::vec2(Input::GetMouseDeltaX(), Input::GetMouseDeltaY());
}

void Input::LockMouseMovement(bool lock)
{
    Input *inp = Input::GetInstance();
    inp->lockMouseMovement = lock;
}

bool Input::IsLockMouseMovement()
{
    Input *inp = Input::GetInstance();
    return inp->lockMouseMovement;
}

glm::vec2 Input::GetMouseCoords()
{
    Input *inp = Input::GetInstance();
    return inp->mouseCoords;
}
