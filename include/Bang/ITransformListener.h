#ifndef ITRANSFORMLISTENER_H
#define ITRANSFORMLISTENER_H

#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

class ITransformListener : public virtual IEventListener
{
public:
    virtual ~ITransformListener() = default;
    virtual void OnTransformChanged()         {}
    virtual void OnParentTransformChanged()   {}
    virtual void OnChildrenTransformChanged() {}
};

NAMESPACE_BANG_END

#endif // ITRANSFORMLISTENER_H
