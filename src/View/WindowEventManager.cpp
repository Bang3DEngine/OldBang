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

void WindowEventManager::RemoveListener(IWindowEventManagerListener *listener)
{
    for(auto it = listeners.begin(); it != listeners.end(); ++it)
    {
        if((*it) == listener)
        {
            listeners.erase(it);
            break;
        }
    }
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

void WindowEventManager::NotifyMenuBarActionClicked(MenuBar::Action clickedAction)
{
    PROPAGATE_EVENT_PAR(OnMenuBarActionClicked(clickedAction), wem->listeners);
}

void WindowEventManager::NotifyInspectorSlotChanged(Part *updatedPart, InspectorPartWidget *inspectorItem)
{
    updatedPart->OnInspectorSlotChanged(inspectorItem);
}

void WindowEventManager::NotifyHierarchyEntitiesSelected(const std::list<Entity*> &selectedEntities)
{
    PROPAGATE_EVENT_PAR(OnTreeHierarchyEntitiesSelected(selectedEntities), wem->listeners);
}


