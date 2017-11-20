#ifndef IDESTROYLISTENER_H
#define IDESTROYLISTENER_H

#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Object;

class IDestroyListener : public IEventListener
{
public:
    virtual ~IDestroyListener() = default;

    virtual void OnDestroyed(Object *object) = 0;

protected:
    IDestroyListener() = default;
};

NAMESPACE_BANG_END

#endif // IDESTROYLISTENER_H

