#include "WindowEventManager.h"

#include "GameObject.h"
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

void WindowEventManager::NotifyChildAdded(GameObject *child)
{
    PROPAGATE_EVENT_PAR(OnChildAdded(child), wem->listeners);
}

void WindowEventManager::NotifyChildChangedParent(GameObject *child, GameObject *previousParent)
{
    PROPAGATE_EVENT_PAR(OnChildChangedParent(child, previousParent), wem->listeners);
}

void WindowEventManager::NotifyChildRemoved(GameObject *child)
{
    PROPAGATE_EVENT_PAR(OnChildRemoved(child), wem->listeners);
}

void WindowEventManager::NotifyMenuBarActionClicked(MenuBar::Action clickedAction)
{
    PROPAGATE_EVENT_PAR(OnMenuBarActionClicked(clickedAction), wem->listeners);
}

void WindowEventManager::NotifyButtonTranslateModeSelected()
{
    PROPAGATE_EVENT_PAR(OnButtonTranslateModeSelected(), wem->listeners);
}

void WindowEventManager::NotifyButtonRotateModeSelected()
{
    PROPAGATE_EVENT_PAR(OnButtonRotateModeSelected(), wem->listeners);
}

void WindowEventManager::NotifyButtonScaleModeSelected()
{
    PROPAGATE_EVENT_PAR(OnButtonScaleModeSelected(), wem->listeners);
}

void WindowEventManager::NotifyInspectorSlotChanged(Component *updatedComponent, InspectorWidget *inspectorItem)
{
    updatedComponent->OnInspectorSlotChanged(inspectorItem);
}

void WindowEventManager::NotifyHierarchyGameObjectsSelected(std::list<GameObject*> &selectedGameObjects)
{
    PROPAGATE_EVENT_PAR(OnTreeHierarchyGameObjectsSelected(selectedGameObjects), wem->listeners);
}

void WindowEventManager::NotifyHierarchyGameObjectDoubleClicked(GameObject *selected)
{
    PROPAGATE_EVENT_PAR(OnTreeHierarchyGameObjectDoubleClicked(selected), wem->listeners);
}


