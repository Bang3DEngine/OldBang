#ifndef ICOMPONENTLISTENER_H
#define ICOMPONENTLISTENER_H

#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Component;

class IComponentListener: public virtual IEventListener
{
    EVENTLISTENER(IComponentListener)

public:
    virtual void OnComponentAdded(Component *addedComponent, int index) {}
    virtual void OnComponentRemoved(Component *removedComponent) {}
};

NAMESPACE_BANG_END

#endif // ICOMPONENTLISTENER_H
