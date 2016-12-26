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

void IWindowEventManagerListener::OnMenuBarActionClicked(MenuBar::Action clickedAction)
{
}
void IWindowEventManagerListener::OnChildAdded(GameObject *child)
{
}
void IWindowEventManagerListener::OnChildChangedParent(GameObject *child, GameObject *previousParent)
{
}
void IWindowEventManagerListener::OnChildRemoved(GameObject *child)
{
}

void IWindowEventManagerListener::OnButtonTranslateModeSelected()
{
}
void IWindowEventManagerListener::OnButtonRotateModeSelected()
{
}
void IWindowEventManagerListener::OnButtonScaleModeSelected()
{
}

void IWindowEventManagerListener::OnGameObjectDestroyed(GameObject *destroyed)
{
}
void IWindowEventManagerListener::OnInspectorSlotChanged(InspectorWidget *inspectorItem)
{
}
void IWindowEventManagerListener::OnHierarchyGameObjectsSelected(List<GameObject *> &selectedGameObjects)
{
}
void IWindowEventManagerListener::OnHierarchyGameObjectDoubleClicked(GameObject *gameObject)
{
}

