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

void IWindowEventManagerListener::OnChildAdded(GameObject *child)
{
}
void IWindowEventManagerListener::OnChildChangedParent(
        GameObject *child, GameObject *previousParent)
{
}
void IWindowEventManagerListener::OnChildRemoved(GameObject *child)
{
}
void IWindowEventManagerListener::OnGameObjectDestroyed(GameObject *destroyed)
{
}

void IWindowEventManagerListener::OnFilenameChanged(
        const String &absFilepathBefore, const String &absFilepathNow)
{
}
void IWindowEventManagerListener::OnInspectorSlotChanged(
        InspectorWidget *inspectorItem)
{
}
void IWindowEventManagerListener::OnHierarchyGameObjectsSelected(
        List<GameObject *> &selectedGameObjects)
{
}
void IWindowEventManagerListener::OnHierarchyGameObjectDoubleClicked(
        GameObject *gameObject)
{
}

