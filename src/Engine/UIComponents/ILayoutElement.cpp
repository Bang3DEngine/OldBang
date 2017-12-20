#include "Bang/ILayoutElement.h"

#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

ILayoutElement::ILayoutElement() {}
ILayoutElement::~ILayoutElement() {}

void ILayoutElement::SetCalculatedLayout(Axis axis, int min,
                                         int preferred,
                                         int flexible)
{
    if (axis == Axis::Horizontal)
    {
        m_calculatedMinSize.x       = min;
        m_calculatedPreferredSize.x = preferred;
        m_calculatedFlexibleSize.x  = flexible;
    }
    else if (axis == Axis::Vertical)
    {
        m_calculatedMinSize.y       = min;
        m_calculatedPreferredSize.y = preferred;
        m_calculatedFlexibleSize.y  = flexible;
    }
}

void ILayoutElement::SetLayoutPriority(int layoutPriority)
{
    m_layoutPriority = layoutPriority;
}

void ILayoutElement::Invalidate()
{
    IInvalidatable<ILayoutElement>::Invalidate();
}

void ILayoutElement::OnInvalidated()
{
    IInvalidatable<ILayoutElement>::OnInvalidated();
    UILayoutManager::PropagateInvalidation(this);
}

int ILayoutElement::GetLayoutPriority() const
{
    return m_layoutPriority;
}

void ILayoutElement::_CalculateLayout(Axis axis)
{
    if (IInvalidatable<ILayoutElement>::IsInvalid())
    {
        CalculateLayout(axis);
        if (axis == Axis::Vertical) { Validate(); }
    }
}

Vector2i ILayoutElement::GetMinSize() const
{
    return m_calculatedMinSize;
}

Vector2i ILayoutElement::GetPreferredSize() const
{
    return m_calculatedPreferredSize;
}

Vector2 ILayoutElement::GetFlexibleSize() const
{
    return m_calculatedFlexibleSize;
}

Vector2 ILayoutElement::GetSize(LayoutSizeType sizeType) const
{
    if (sizeType == LayoutSizeType::Min) { return Vector2( GetMinSize() ); }
    if (sizeType == LayoutSizeType::Preferred) { return Vector2( GetPreferredSize() ); }
    if (sizeType == LayoutSizeType::Flexible)  { return GetFlexibleSize(); }
    return Vector2::Zero;
}
