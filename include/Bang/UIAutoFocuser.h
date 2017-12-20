#ifndef UIAUTOFOCUSER_H
#define UIAUTOFOCUSER_H

#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class IFocusable;

class UIAutoFocuser : public Component
{
    COMPONENT(UIAutoFocuser)

public:
    // Component
    void OnStart() override;

    void SetFocusableToAutoFocus(IFocusable* focusable);
    IFocusable* GetFocusableToAutoFocus() const;

private:
    IFocusable* p_focusableToAutoFocus = nullptr;

    UIAutoFocuser() = default;
    virtual ~UIAutoFocuser() = default;
};

NAMESPACE_BANG_END

#endif // UIAUTOFOCUSER_H

