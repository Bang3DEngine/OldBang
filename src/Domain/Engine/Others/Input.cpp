#include "Input.h"

#include "Canvas.h"

std::map<Input::Key, Input::ButtonInfo> Input::keyInfos;

float Input::lastMouseWheelDelta = 0.0f;
bool Input::lockMouseMovement = false;
int Input::framesMouseStopped = 0;
glm::vec2 Input::mouseCoords = glm::vec2(0.0f);
glm::vec2 Input::lastMouseCoords = glm::vec2(0.0f);
std::map<Input::MouseButton, Input::ButtonInfo> Input::mouseInfo;

Input::Input() {} //private, never called

void Input::Init() //called from Canvas
{
}

void Input::OnNewFrame()
{
    for(auto it = keyInfos.begin(); it != keyInfos.end(); ++it)
    {
        ButtonInfo &kInfo = it->second;
        if(kInfo.up) //After a frame where it was Up
        {
            keyInfos.erase(it);
        }
        else if(kInfo.down)
        {
            kInfo.down = false; //Not down anymore, just pressed.
        }
    }

    for(auto it = mouseInfo.begin(); it != mouseInfo.end(); ++it)
    {
        ButtonInfo &mbInfo = it->second;
        if(mbInfo.up) //After a frame where it was Up
        {
            mouseInfo.erase(it);
        }
        else if(mbInfo.down)
        {
            mbInfo.down = false; //Not down anymore, just pressed.
        }
    }

    lastMouseWheelDelta = 0.0f;
    if(!lockMouseMovement)
    {
        lastMouseCoords = mouseCoords;
    }
    else if(framesMouseStopped > 3)
    {
        //For the case of mouse movement locking, this
        //avoids a bug of axisX/Y getting stuck when mouse
        //does: lock/moving/stop_moving => axisX/Y think mouse is still moving
        lastMouseCoords = mouseCoords;
    }
    ++framesMouseStopped;


   // Logger_Log("Locked: " << lockMouseMovement);
   // Logger_Log("Axis: (" << GetMouseAxisX() << ", " << GetMouseAxisY() << ")");
   // Logger_Log("---------------------------------");
}

void Input::HandleInputMousWheel(QWheelEvent *event)
{
    lastMouseWheelDelta = float(event->delta()) / (24.0f * 15.0f);
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

    /*Logger_Log("MMEvent: lastMouseCoords" << lastMouseCoords << ", evxy:" << glm::vec2(event->x(), event->y()) );
    if(lockMouseMovement &&
       lastMouseCoords == glm::vec2(event->x(), event->y()))
    {
        fakeCall = false;
        return;
    }*/

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
    {   //Only if it was not down/pressed before
        mouseInfo[mb] = ButtonInfo(false, true, true);
    }
}

void Input::HandleInputMouseRelease(QMouseEvent *event)
{
    MouseButton mb = static_cast<MouseButton>(event->button());
    if(mouseInfo.find(mb) != mouseInfo.end() &&
       mouseInfo[mb].pressed)
    {   //Only if it was pressed before
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


bool Input::GetKey(Input::Key k)
{
    return keyInfos.find(k) != keyInfos.end() &&
           keyInfos[k].pressed;
}

bool Input::GetKeyUp(Input::Key k)
{
    return keyInfos.find(k) != keyInfos.end() &&
           keyInfos[k].up;
}

bool Input::GetKeyDown(Input::Key k)
{
    return keyInfos.find(k) != keyInfos.end() &&
            keyInfos[k].down;
}

float Input::GetMouseWheel()
{
    return lastMouseWheelDelta;
}


bool Input::GetMouseButton(Input::MouseButton mb)
{
    return mouseInfo.find(mb) != mouseInfo.end() &&
           mouseInfo[mb].pressed;
}

bool Input::GetMouseButtonUp(Input::MouseButton mb)
{
    return mouseInfo.find(mb) != mouseInfo.end() &&
           mouseInfo[mb].up;
}

bool Input::GetMouseButtonDown(Input::MouseButton mb)
{
    return mouseInfo.find(mb) != mouseInfo.end() &&
            mouseInfo[mb].down;
}

float Input::GetMouseAxisX()
{
    return GetMouseDeltaX() / Canvas::GetWidth();
}

float Input::GetMouseAxisY()
{
    return GetMouseDeltaY() / Canvas::GetHeight();
}

float Input::GetMouseDeltaX()
{
    return mouseCoords.x - lastMouseCoords.x;
}

float Input::GetMouseDeltaY()
{
    return mouseCoords.y - lastMouseCoords.y;
}

void Input::LockMouseMovement(bool lock)
{
    lockMouseMovement = lock;
}

bool Input::IsLockMouseMovement()
{
    return lockMouseMovement;
}

glm::vec2 Input::GetMouseCoords()
{
    return mouseCoords;
}
