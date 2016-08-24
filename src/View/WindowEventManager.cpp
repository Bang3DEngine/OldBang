#include "WindowEventManager.h"

#include "GameObject.h"
#include "EditorWindow.h"
#include "InspectorWidget.h"
#include "IWindowEventManagerListener.h"

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
        s_wem = EditorWindow::GetInstance()->windowEventManager;
    }

    return s_wem;
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

void WindowEventManager::NotifyInspectorSlotChanged(InspectorWidget *inspectorItem)
{
    PROPAGATE_EVENT_PAR(OnInspectorSlotChanged(inspectorItem), s_wem->m_listeners);
}

void WindowEventManager::NotifyHierarchyGameObjectsSelected(std::list<GameObject*> &selectedGameObjects)
{
    PROPAGATE_EVENT_PAR(OnTreeHierarchyGameObjectsSelected(selectedGameObjects), s_wem->m_listeners);
}

void WindowEventManager::NotifyHierarchyGameObjectDoubleClicked(GameObject *selected)
{
    PROPAGATE_EVENT_PAR(OnTreeHierarchyGameObjectDoubleClicked(selected), s_wem->m_listeners);
}


