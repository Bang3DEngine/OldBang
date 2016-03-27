#ifndef IWINDOWEVENTMANAGERLISTENER_H
#define IWINDOWEVENTMANAGERLISTENER_H

#include "Bang.h"
#include <list>

class Entity;
class InspectorPartWidget;
class IWindowEventManagerListener
{
protected:
    IWindowEventManagerListener();
    virtual ~IWindowEventManagerListener();

public:
    virtual void OnMenuBarActionClicked(int clickedAction) { }
    virtual void OnChildAdded(Entity *child) { }
    virtual void OnChildChangedParent(Entity *child, Entity *previousParent) { }
    virtual void OnChildRemoved(Entity *child) { }

    virtual void OnInspectorSlotChanged(InspectorPartWidget *inspectorItem)  { }
    virtual void OnTreeHierarchyEntitiesSelected(const std::list<Entity*> &selectedEntities) { }
};

#endif // IWINDOWEVENTMANAGERLISTENER_H
