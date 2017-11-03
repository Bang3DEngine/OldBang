#ifndef IENABLEDLISTENER_H
#define IENABLEDLISTENER_H

#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

class IEnabledListener : public IEventListener
{
public:
    virtual void OnEnabled()  {}
    virtual void OnDisabled() {}
};

NAMESPACE_BANG_END

#endif // IENABLEDLISTENER_H
