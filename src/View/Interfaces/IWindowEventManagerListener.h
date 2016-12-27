#ifndef IWINDOWEVENTMANAGERLISTENER_H
#define IWINDOWEVENTMANAGERLISTENER_H

#include "List.h"

#include "MenuBar.h"

class GameObject;
class InspectorWidget;
class IWindowEventManagerListener
{
protected:
    IWindowEventManagerListener();
    virtual ~IWindowEventManagerListener();

public:
    virtual void OnMenuBarActionClicked(MenuBar::Action clickedAction);
    virtual void OnChildAdded(GameObject *child);
    virtual void OnChildChangedParent(GameObject *child, GameObject *previousParent);
    virtual void OnChildRemoved(GameObject *child);

    virtual void OnButtonTranslateModeSelected();
    virtual void OnButtonRotateModeSelected();
    virtual void OnButtonScaleModeSelected();

    virtual void OnGameObjectDestroyed(GameObject *destroyed);

    virtual void OnInspectorSlotChanged(InspectorWidget *inspectorItem);
    virtual void OnHierarchyGameObjectsSelected(List<GameObject*> &selectedGameObjects);
    virtual void OnHierarchyGameObjectDoubleClicked(GameObject* gameObject);
};

#endif // IWINDOWEVENTMANAGERLISTENER_H
