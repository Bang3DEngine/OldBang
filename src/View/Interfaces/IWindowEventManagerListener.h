#ifndef IWINDOWEVENTMANAGERLISTENER_H
#define IWINDOWEVENTMANAGERLISTENER_H

#include <list>

#include "Bang.h"

#include "MenuBar.h"

class GameObject;
class InspectorWidget;
class IWindowEventManagerListener
{
protected:
    IWindowEventManagerListener();
    virtual ~IWindowEventManagerListener();

public:
    virtual void OnMenuBarActionClicked(MenuBar::Action clickedAction) { }
    virtual void OnChildAdded(GameObject *child) { }
    virtual void OnChildChangedParent(GameObject *child, GameObject *previousParent) { }
    virtual void OnChildRemoved(GameObject *child) { }

    virtual void OnButtonTranslateModeSelected() {}
    virtual void OnButtonRotateModeSelected() {}
    virtual void OnButtonScaleModeSelected() {}

    virtual void OnInspectorSlotChanged(InspectorWidget *inspectorItem)  { }
    virtual void OnTreeHierarchyGameObjectsSelected(std::list<GameObject*> &selectedGameObjects) { }
    virtual void OnTreeHierarchyGameObjectDoubleClicked(GameObject *selected) { }
};

#endif // IWINDOWEVENTMANAGERLISTENER_H
