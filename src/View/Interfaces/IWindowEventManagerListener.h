#ifndef IWINDOWEVENTMANAGERLISTENER_H
#define IWINDOWEVENTMANAGERLISTENER_H

#include "Bang.h"
#include <list>

class Entity;
class ListInspectorItemWidget;
class IWindowEventManagerListener
{
protected:
    IWindowEventManagerListener();

public:
    virtual void OnMenuBarItemClicked(int itemClicked) { }
    virtual void OnChildAdded(Entity *child) { }
    virtual void OnChildChangedParent(Entity *child, Entity *previousParent) { }
    virtual void OnChildRemoved(Entity *child) { }

    virtual void OnInspectorSlotChanged(ListInspectorItemWidget *inspectorItem)  { }
    virtual void OnTreeHierarchyEntitiesSelected(const std::list<Entity*> &selectedEntities) { }
};

#endif // IWINDOWEVENTMANAGERLISTENER_H
