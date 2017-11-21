#ifndef ICREATELISTENER_H
#define ICREATELISTENER_H
#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Object;

class ICreateListener : public virtual IEventListener
{
public:
    virtual ~ICreateListener() = default;

    virtual void OnCreated(Object *object) = 0;

protected:
    ICreateListener() = default;
};

NAMESPACE_BANG_END

#endif // ICREATELISTENER_H
