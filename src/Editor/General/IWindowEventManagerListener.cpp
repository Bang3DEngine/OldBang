#include "Bang/IWindowEventManagerListener.h"

#include "Bang/WindowEventManager.h"

IWindowEventManagerListener::IWindowEventManagerListener()
{
    ENSURE(WindowEventManager::GetInstance());
    WindowEventManager::GetInstance()->AddListener(this);
}

IWindowEventManagerListener::~IWindowEventManagerListener()
{
    ENSURE(WindowEventManager::GetInstance());
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

void IWindowEventManagerListener::OnHierarchyGameObjectsSelected(
        const List<GameObject *> &selectedGameObjects)
{
}
void IWindowEventManagerListener::OnHierarchyGameObjectDoubleClicked(
        GameObject *gameObject)
{
}

