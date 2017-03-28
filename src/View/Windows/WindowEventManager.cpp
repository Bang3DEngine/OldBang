#include "Bang/WindowEventManager.h"

#include "Bang/GameObject.h"
#include "Bang/EditorWindow.h"
#include "Bang/InspectorWidget.h"
#include "Bang/IWindowEventManagerListener.h"

WindowEventManager *WindowEventManager::s_wem = nullptr;

WindowEventManager::WindowEventManager(QWidget *parent) : QGLWidget(parent)
{

}

void WindowEventManager::AddListener(IWindowEventManagerListener *listener)
{
    m_listeners.PushBack(listener);
}

void WindowEventManager::RemoveListener(IWindowEventManagerListener *listener)
{
    m_listeners.Remove(listener);
}

WindowEventManager *WindowEventManager::GetInstance()
{
    if (!WindowEventManager::s_wem)
    {
        WindowEventManager::s_wem =
                EditorWindow::GetInstance()->m_winEventManager;
    }
    return WindowEventManager::s_wem;
}

void WindowEventManager::NotifyGameObjectDestroyed(GameObject *destroyed)
{
    PROPAGATE_EVENT_PAR(OnGameObjectDestroyed(destroyed), s_wem->m_listeners);
}

void WindowEventManager::NotifyInspectorSlotChanged(InspectorWidget *inspectorItem)
{
    PROPAGATE_EVENT_PAR(OnInspectorSlotChanged(inspectorItem), s_wem->m_listeners);
}

void WindowEventManager::NotifyHierarchyGameObjectsSelected(List<GameObject*> &selectedGameObjects)
{
    PROPAGATE_EVENT_PAR(OnHierarchyGameObjectsSelected(selectedGameObjects), s_wem->m_listeners);
}

void WindowEventManager::NotifyHierarchyGameObjectDoubleClicked(GameObject *selected)
{
    PROPAGATE_EVENT_PAR(OnHierarchyGameObjectDoubleClicked(selected), s_wem->m_listeners);
}


