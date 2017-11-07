#ifndef UIFOCUSABLE_H
#define UIFOCUSABLE_H

#include "Bang/Component.h"
#include "Bang/IFocusListener.h"
#include "Bang/EventEmitterComponent.h"

NAMESPACE_BANG_BEGIN

class UIFocusable : public Component,
                    public EventEmitterComponent<IFocusListener>
{
    COMPONENT(UIFocusable)

public:
	UIFocusable();
	virtual ~UIFocusable();

    void OnUpdate() override;

    void LeaveFocus();
    bool HasFocus() const;
    bool HasJustFocusChanged() const;

private:
    bool m_hasFocus = false;
    bool m_hasJustFocusChanged = false;

    void PropagateToFocusListeners();
    void HandleFocusing(bool handleFocusTake);
};

NAMESPACE_BANG_END

#endif // UIFOCUSABLE_H

