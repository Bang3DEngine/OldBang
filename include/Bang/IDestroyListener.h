#ifndef IDESTROYLISTENER_H
#define IDESTROYLISTENER_H

#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

class IDestroyListener : public virtual IEventListener
{
    EVENTLISTENER(IDestroyListener)

public:
    virtual void OnDestroyed(EventEmitter<IDestroyListener> *object) = 0;
};

NAMESPACE_BANG_END

#endif // IDESTROYLISTENER_H

