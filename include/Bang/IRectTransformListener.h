#ifndef IRECTTRANSFORMLISTENER_H
#define IRECTTRANSFORMLISTENER_H

#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

class IRectTransformListener : public IEventListener
{
public:
    virtual void OnRectTransformChanged()         {}
    virtual void OnParentRectTransformChanged()   {}
    virtual void OnChildrenRectTransformChanged() {}
};

NAMESPACE_BANG_END

#endif // IRECTTRANSFORMLISTENER_H
