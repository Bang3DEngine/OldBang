#include "IWindowEventManagerListener.h"

#include "WindowEventManager.h"

IWindowEventManagerListener::IWindowEventManagerListener()
{
    WindowEventManager::GetInstance()->AddListener(this);
}
