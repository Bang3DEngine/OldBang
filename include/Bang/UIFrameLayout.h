#ifndef UIFRAMELAYOUT_H
#define UIFRAMELAYOUT_H

#include "Bang/UIGroupLayout.h"

NAMESPACE_BANG_BEGIN

class UIFrameLayout : public UIGroupLayout
{
    COMPONENT(UIFrameLayout);

public:
    UIFrameLayout();
    virtual ~UIFrameLayout();

    // ILayoutController
    virtual void ApplyLayout() override;

    // ILayoutElement
    virtual Vector2i CalculateTotalMinSize() const override;
    virtual Vector2i CalculateTotalPreferredSize() const override;
    virtual Vector2i CalculateTotalFlexiblePxSize() const override;
};

NAMESPACE_BANG_END

#endif // UIFRAMELAYOUT_H
