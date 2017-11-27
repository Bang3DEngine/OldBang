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
    void OnUpdate() override;
    void OnPostUpdate() override;

    bool HasMouseOver() const;

    void SetFocusEnabled(bool focusEnabled);

    bool HasFocus() const;
    bool IsFocusEnabled() const;
    bool HasJustFocusChanged() const;

protected:
    virtual bool CanBeRepeatedInGameObject() const override;

private:
    bool m_hasFocus = false;
    bool m_focusEnabled = true;
    bool m_hasMouseOver = false;
    bool m_hasJustFocusChanged = false;

    UIFocusable();
    virtual ~UIFocusable();

    void PropagateToFocusListeners();
    void HandleFocusing(bool handleFocusTake);

    void SetFocus();
    void ClearFocus();

    friend class UICanvas;
};

NAMESPACE_BANG_END

#endif // UIFOCUSABLE_H

