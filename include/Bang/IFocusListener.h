#ifndef IFOCUSLISTENER_H
#define IFOCUSLISTENER_H

#include "Bang/Bang.h"
#include "Bang/FocusAction.h"
#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

class IFocusListener : public virtual IEventListener
{
public:
    virtual void OnFocusTaken() {}
    virtual void OnFocusLost()  {}

protected:
    IFocusListener() = default;
};

NAMESPACE_BANG_END

#endif // IFOCUSLISTENER_H
