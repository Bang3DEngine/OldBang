#include "WindowEventManager.h"

#include "Entity.h"
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

void WindowEventManager::NotifyChildAdded(Entity *child)
{
    PROPAGATE_EVENT_PAR(OnChildAdded(child), wem->listeners);
}

void WindowEventManager::NotifyMenuBarItemClicked(int itemClicked)
{
    PROPAGATE_EVENT_PAR(OnMenuBarItemClicked(itemClicked), wem->listeners);
}

void WindowEventManager::_NotifyMenuBarItemClicked(int itemClicked)
{
    WindowEventManager::NotifyMenuBarItemClicked(itemClicked);
}


