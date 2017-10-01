#include "Bang/ILayoutElement.h"

#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

ILayoutElement::ILayoutElement()
{

}

ILayoutElement::~ILayoutElement()
{

}

void ILayoutElement::SetInvalid(bool invalid)
{
    m_isInvalid = invalid;
}
bool ILayoutElement::IsInvalid() const
{
    return m_isInvalid;
}

void ILayoutElement::SetLayoutPriority(int layoutPriority)
{
    m_layoutPriority = layoutPriority;
}

int ILayoutElement::GetLayoutPriority() const
{
    return m_layoutPriority;
}

Vector2i ILayoutElement::GetTotalMinSize() const
{
    CalculateTotalSizes();
    return m_calculatedTotalMinSize;
}

Vector2i ILayoutElement::GetTotalPreferredSize() const
{
    CalculateTotalSizes();
    return m_calculatedTotalPreferredSize;
}

Vector2 ILayoutElement::GetFlexibleSize() const
{
    return Vector2(1);
}

Vector2 ILayoutElement::GetTotalSize(LayoutSizeType sizeType) const
{
    if (sizeType == LayoutSizeType::Min)
    { return Vector2( GetTotalMinSize() ); }

    if (sizeType == LayoutSizeType::Preferred)
    { return Vector2( GetTotalPreferredSize() ); }

    if (sizeType == LayoutSizeType::Flexible)  { return GetFlexibleSize(); }

    return Vector2::Zero;
}

void ILayoutElement::CalculateTotalSizes() const
{
    if (IsInvalid())
    {
        m_calculatedTotalMinSize        = CalculateTotalMinSize();
        m_calculatedTotalPreferredSize  = CalculateTotalPreferredSize();

        ILayoutElement *ncThis = const_cast<ILayoutElement*>(this);
        ncThis->SetInvalid(false);
    }
}
