#ifndef UIFOCUSABLE_H
#define UIFOCUSABLE_H

#include "Bang/Component.h"
#include "Bang/IFocusable.h"

NAMESPACE_BANG_BEGIN

class UIFocusable : public Component,
                    public IFocusable
{
    COMPONENT(UIFocusable)

public:
    void OnUpdate() override;
    void OnPostUpdate() override;

    bool HasJustFocusChanged() const;

protected:
    // IFocusable
    virtual void SetFocus() override;
    virtual void ClearFocus() override;
    virtual void PropagateToFocusListeners() override;

    // Component
    virtual bool CanBeRepeatedInGameObject() const override;

private:
    bool m_hasJustFocusChanged = false;

    UIFocusable();
    virtual ~UIFocusable();
};

NAMESPACE_BANG_END

#endif // UIFOCUSABLE_H

