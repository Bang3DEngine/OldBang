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

private:
    bool m_hasFocus = false;

    void HandleFocusing(bool focusTake);
};

NAMESPACE_BANG_END

#endif // UIFOCUSABLE_H

