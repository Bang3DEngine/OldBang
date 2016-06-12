#include "IWindowEventManagerListener.h"

#include "WindowEventManager.h"

IWindowEventManagerListener::IWindowEventManagerListener()
{
    WindowEventManager::GetInstance()->AddListener(this);
}

IWindowEventManagerListener::~IWindowEventManagerListener()
{
    WindowEventManager::GetInstance()->RemoveListener(this);
}

