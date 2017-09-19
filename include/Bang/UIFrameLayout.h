#ifndef UIFRAMELAYOUT_H
#define UIFRAMELAYOUT_H

#include "Bang/Component.h"
#include "Bang/ILayoutElement.h"
#include "Bang/ILayoutController.h"

NAMESPACE_BANG_BEGIN

class UIFrameLayout : public Component,
                      public ILayoutElement,
                      public ILayoutController
{
    COMPONENT(UIFrameLayout);

public:
    UIFrameLayout();
    virtual ~UIFrameLayout();

    // ILayoutController
    virtual void ApplyLayoutToChildren() override;

    // ILayoutElement
    virtual Vector2i CalculateTotalMinSize() const override;
    virtual Vector2i CalculateTotalPreferredSize() const override;
    virtual Vector2i CalculateTotalFlexiblePxSize() const override;
};

NAMESPACE_BANG_END

#endif // UIFRAMELAYOUT_H
