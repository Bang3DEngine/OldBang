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
    virtual void OnChildAdded(GameObject *child);
    virtual void OnChildChangedParent(GameObject *child,
                                      GameObject *previousParent);
    virtual void OnChildRemoved(GameObject *child);

    virtual void OnGameObjectDestroyed(GameObject *destroyed);

    virtual void OnFilenameChanged(const String &absFilepathBefore,
                                   const String &absFilepathNow);

    virtual void OnInspectorSlotChanged(InspectorWidget *inspectorItem);
    virtual void OnHierarchyGameObjectsSelected(
                                    List<GameObject*> &selectedGameObjects);
    virtual void OnHierarchyGameObjectDoubleClicked(GameObject* gameObject);
};

#endif // IWINDOWEVENTMANAGERLISTENER_H
