#ifndef IWINDOWEVENTMANAGERLISTENER_H
#define IWINDOWEVENTMANAGERLISTENER_H

#include "Bang.h"

class Entity;
class IWindowEventManagerListener
{
protected:
    IWindowEventManagerListener();

public:
    virtual void OnMenuBarItemClicked(int itemClicked) { }
    virtual void OnChildAdded(Entity *child) { }
    virtual void OnChildRemoved(Entity *child) { }

    virtual void OnTreeHierarchyEntitySelected(Entity *e) { }
};

#endif // IWINDOWEVENTMANAGERLISTENER_H
