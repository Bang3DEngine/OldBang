#ifndef IWINDOWEVENTMANAGERLISTENER_H
#define IWINDOWEVENTMANAGERLISTENER_H

class Entity;
class IWindowEventManagerListener
{
protected:
    IWindowEventManagerListener();

public:
    virtual void OnMenuBarItemClicked(int itemClicked) { };
    virtual void OnChildAdded(Entity *child) { };
};

#endif // IWINDOWEVENTMANAGERLISTENER_H
