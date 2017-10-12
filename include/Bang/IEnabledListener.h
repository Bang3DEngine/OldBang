#ifndef IENABLEDLISTENER_H
#define IENABLEDLISTENER_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class IEnabledListener
{
public:
    virtual void OnEnabled()  {}
    virtual void OnDisabled() {}
};

NAMESPACE_BANG_END

#endif // IENABLEDLISTENER_H
