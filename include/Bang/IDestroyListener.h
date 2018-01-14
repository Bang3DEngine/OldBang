#ifndef IDESTROYLISTENER_H
#define IDESTROYLISTENER_H

#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

class IDestroyListener : public virtual IEventListener
{
public:
    virtual ~IDestroyListener() = default;

    virtual void OnDestroyed(EventEmitter<IDestroyListener> *object) = 0;

protected:
    IDestroyListener() = default;
};

NAMESPACE_BANG_END

#endif // IDESTROYLISTENER_H

