#ifndef IRECTTRANSFORMLISTENER_H
#define IRECTTRANSFORMLISTENER_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class IRectTransformListener
{
public:
    virtual void OnRectTransformChanged()       {}
    virtual void OnParentRectTransformChanged() {}
};

NAMESPACE_BANG_END

#endif // IRECTTRANSFORMLISTENER_H
