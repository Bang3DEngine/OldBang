#include "Bang/UIFrameLayout.h"

#include "Bang/List.h"
#include "Bang/GameObject.h"

USING_NAMESPACE_BANG

UIFrameLayout::UIFrameLayout()
{

}

UIFrameLayout::~UIFrameLayout()
{

}

void UIFrameLayout::ApplyLayoutToChildren()
{

}

Vector2i UIFrameLayout::CalculateTotalMinSize() const
{
    ILayoutElement* layoutElm =
            gameObject->GetComponentInChildrenOnly<ILayoutElement>(false);
    if (layoutElm) { return layoutElm->GetTotalMinSize(); }
    return Vector2i::Zero;
}

Vector2i UIFrameLayout::CalculateTotalPreferredSize() const
{
    ILayoutElement* layoutElm =
            gameObject->GetComponentInChildrenOnly<ILayoutElement>(false);
    if (layoutElm) { return layoutElm->GetTotalPreferredSize(); }
    return Vector2i::Zero;
}

Vector2i UIFrameLayout::CalculateTotalFlexiblePxSize() const
{
    ILayoutElement* layoutElm =
            gameObject->GetComponentInChildrenOnly<ILayoutElement>(false);
    if (layoutElm) { return layoutElm->GetTotalFlexiblePxSize(); }
    return Vector2i::Zero;
}
