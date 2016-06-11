#include "IWindowEventManagerListener.h"

#include "WindowEventManager.h"

IWindowEventManagerListener::IWindowEventManagerListener()
{
    Logger_Log("Registering..." << (void*)this << " to " << (void*)(WindowEventManager::GetInstance()));
    WindowEventManager::GetInstance()->AddListener(this);
}

IWindowEventManagerListener::~IWindowEventManagerListener()
{
    WindowEventManager::GetInstance()->RemoveListener(this);
}

