#ifndef IOBJECTLISTENER_H
#define IOBJECTLISTENER_H

#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

class IObjectListener : public virtual IEventListener
{
public:
    virtual void OnStarted()  {}
    virtual void OnEnabled()  {}
    virtual void OnDisabled() {}
};

NAMESPACE_BANG_END

#endif // IOBJECTLISTENER_H
