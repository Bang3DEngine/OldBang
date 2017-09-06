#ifndef IFOCUSLISTENER_H
#define IFOCUSLISTENER_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

enum class FocusAction { TakeIt, PassToParent };

class IFocusListener
{
public:
    virtual void OnFocusTaken() {}
    virtual void OnFocusLost()  {}
};

NAMESPACE_BANG_END

#endif // IFOCUSLISTENER_H
