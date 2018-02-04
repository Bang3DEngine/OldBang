#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

UIHorizontalLayout::UIHorizontalLayout() : UIDirLayout(Axis::Horizontal)
{
    SetChildrenHorizontalStretch(Stretch::None);
    SetChildrenVerticalStretch(Stretch::Full);
}
