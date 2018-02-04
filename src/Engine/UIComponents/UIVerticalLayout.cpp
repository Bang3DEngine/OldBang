#include "Bang/UIVerticalLayout.h"

USING_NAMESPACE_BANG

UIVerticalLayout::UIVerticalLayout() : UIDirLayout(Axis::Vertical)
{
    SetChildrenHorizontalStretch(Stretch::Full);
    SetChildrenVerticalStretch(Stretch::None);
}
