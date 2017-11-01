#ifndef UIFOCUSABLE_H
#define UIFOCUSABLE_H

#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

class UIFocusable : public Component
{
    COMPONENT(UIFocusable)

public:
	UIFocusable();
	virtual ~UIFocusable();

    void OnPreUpdate() override;
    void OnUpdate() override;

    bool HasFocus() const;
    bool HasJustFocusChanged() const;

private:
    bool m_hasFocus = false;
    bool m_hasJustFocusChanged = false;

    void HandleFocusing(bool handleFocusTake);
};

NAMESPACE_BANG_END

#endif // UIFOCUSABLE_H

