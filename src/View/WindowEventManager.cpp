#include "WindowEventManager.h"

#include "GameObject.h"
#include "WindowMain.h"

WindowEventManager *WindowEventManager::s_wem = nullptr;

WindowEventManager::WindowEventManager(QWidget *parent) : QGLWidget(parent)
{

}

void WindowEventManager::AddListener(IWindowEventManagerListener *listener)
{
    m_listeners.push_back(listener);
}

void WindowEventManager::RemoveListener(IWindowEventManagerListener *listener)
{
    for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
    {
        if ((*it) == listener)
        {
            m_listeners.erase(it);
            break;
        }
    }
}

WindowEventManager *WindowEventManager::GetInstance()
{
    if (!s_wem)
    {
        s_wem = WindowMain::GetInstance()->windowEventManager;
    }

    return s_wem;
}

void WindowEventManager::NotifyChildAdded(GameObject *child)
{
    PROPAGATE_EVENT_PAR(OnChildAdded(child), s_wem->m_listeners);
}

void WindowEventManager::NotifyChildChangedParent(GameObject *child, GameObject *previousParent)
{
    PROPAGATE_EVENT_PAR(OnChildChangedParent(child, previousParent), s_wem->m_listeners);
}

void WindowEventManager::NotifyChildRemoved(GameObject *child)
{
    PROPAGATE_EVENT_PAR(OnChildRemoved(child), s_wem->m_listeners);
}

void WindowEventManager::NotifyMenuBarActionClicked(MenuBar::Action clickedAction)
{
    PROPAGATE_EVENT_PAR(OnMenuBarActionClicked(clickedAction), s_wem->m_listeners);
}

void WindowEventManager::NotifyButtonTranslateModeSelected()
{
    PROPAGATE_EVENT_PAR(OnButtonTranslateModeSelected(), s_wem->m_listeners);
}

void WindowEventManager::NotifyButtonRotateModeSelected()
{
    PROPAGATE_EVENT_PAR(OnButtonRotateModeSelected(), s_wem->m_listeners);
}

void WindowEventManager::NotifyButtonScaleModeSelected()
{
    PROPAGATE_EVENT_PAR(OnButtonScaleModeSelected(), s_wem->m_listeners);
}

void WindowEventManager::NotifyInspectorSlotChanged(Component *updatedComponent, InspectorWidget *inspectorItem)
{
    updatedComponent->OnInspectorSlotChanged(inspectorItem);
}

void WindowEventManager::NotifyHierarchyGameObjectsSelected(std::list<GameObject*> &selectedGameObjects)
{
    PROPAGATE_EVENT_PAR(OnTreeHierarchyGameObjectsSelected(selectedGameObjects), s_wem->m_listeners);
}

void WindowEventManager::NotifyHierarchyGameObjectDoubleClicked(GameObject *selected)
{
    PROPAGATE_EVENT_PAR(OnTreeHierarchyGameObjectDoubleClicked(selected), s_wem->m_listeners);
}


