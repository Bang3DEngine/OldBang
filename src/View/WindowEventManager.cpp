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

void WindowEventManager::NotifyChildChangedParent(Entity *child, Entity *previousParent)
{
    PROPAGATE_EVENT_PAR(OnChildChangedParent(child, previousParent), wem->listeners);
}

void WindowEventManager::NotifyChildRemoved(Entity *child)
{
    PROPAGATE_EVENT_PAR(OnChildRemoved(child), wem->listeners);
}

void WindowEventManager::NotifyMenuBarItemClicked(int itemClicked)
{
    PROPAGATE_EVENT_PAR(OnMenuBarItemClicked(itemClicked), wem->listeners);
}

void WindowEventManager::_NotifyMenuBarItemClicked(int itemClicked)
{
    WindowEventManager::NotifyMenuBarItemClicked(itemClicked);
}

void WindowEventManager::NotifyInspectorSlotChanged(Part *updatedPart)
{
    updatedPart->OnInspectorSlotChanged();
}

void WindowEventManager::NotifyHierarchyEntitiesSelected(const std::list<Entity*> &selectedEntities)
{
    PROPAGATE_EVENT_PAR(OnTreeHierarchyEntitiesSelected(selectedEntities), wem->listeners);
}


