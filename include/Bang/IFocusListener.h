#ifndef IFOCUSLISTENER_H
#define IFOCUSLISTENER_H

#include "Bang/Bang.h"
#include "Bang/FocusAction.h"
#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class IFocusable;

class IFocusListener : public virtual IEventListener
{
public:
    virtual void OnFocusTaken(IFocusable *focusable) { (void)focusable; }
    virtual void  OnFocusLost(IFocusable *focusable) { (void)focusable; }
    virtual void OnMouseEnter(IFocusable *focusable) { (void)focusable; }
    virtual void  OnMouseExit(IFocusable *focusable) { (void)focusable; }

protected:
    IFocusListener() = default;
};

NAMESPACE_BANG_END

#endif // IFOCUSLISTENER_H
