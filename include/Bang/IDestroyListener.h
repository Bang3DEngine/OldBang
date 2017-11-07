#ifndef IDESTROYLISTENER_H
#define IDESTROYLISTENER_H

#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

class IDestroyListener : public IEventListener
{
public:
    virtual ~IDestroyListener() = default;

    virtual void OnBeforeDestroyed(IEventEmitter *destroyedEmitter) = 0;

protected:
    IDestroyListener() = default;
};

NAMESPACE_BANG_END

#endif // IDESTROYLISTENER_H

