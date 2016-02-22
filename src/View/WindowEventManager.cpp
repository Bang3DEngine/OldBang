#include "WindowEventManager.h"

#include "WindowMain.h"

WindowEventManager *WindowEventManager::wem = nullptr;

WindowEventManager::WindowEventManager(QWidget *parent) : QGLWidget(parent)
{

}

void WindowEventManager::AddListener(IWindowEventManagerListener *listener)
{
    listeners.push_back(listener);
}

WindowEventManager *WindowEventManager::GetInstance()
{
    if(wem == nullptr)
    {
        wem = WindowMain::GetInstance()->windowEventManager;
    }

    return wem;
}

void WindowEventManager::OnMenuBarItemClicked(int itemClicked)
{
    Logger_Log("Menu bar item clicked: " << itemClicked);
    for(IWindowEventManagerListener *listener : wem->listeners)
    {
        listener->OnMenuBarItemClicked(itemClicked);
    }
}

void WindowEventManager::_OnMenuBarItemClicked(int itemClicked)
{
    OnMenuBarItemClicked(itemClicked);
}


