#ifndef UIFOCUSABLE_H
#define UIFOCUSABLE_H

#include "Bang/Component.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IFocusListener.h"

NAMESPACE_BANG_BEGIN

class UIFocusable : public Component,
                    public EventEmitter<IFocusListener>
{
    COMPONENT(UIFocusable)

public:
	UIFocusable();
	virtual ~UIFocusable();

    void OnUpdate() override;

    bool IsMouseOver() const;

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

