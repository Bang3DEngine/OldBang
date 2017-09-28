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

void ILayoutElement::SetPriority(int layoutPriority)
{
    m_layoutPriority = layoutPriority;
}

int ILayoutElement::GetPriority() const
{
    return m_layoutPriority;
}

int ILayoutElement::GetTotalMinWidth() const
{
    return GetTotalMinSize().x;
}

int ILayoutElement::GetTotalMinHeight() const
{
    return GetTotalMinSize().y;
}

int ILayoutElement::GetTotalPreferredWidth() const
{
    return GetTotalPreferredSize().x;
}

int ILayoutElement::GetTotalPreferredHeight() const
{
    return GetTotalPreferredSize().y;
}

int ILayoutElement::GetTotalFlexibleWidth() const
{
    return GetTotalFlexiblePxSize().x;
}

int ILayoutElement::GetTotalFlexibleHeight() const
{
    return GetTotalFlexiblePxSize().y;
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

Vector2i ILayoutElement::GetTotalFlexiblePxSize() const
{
    CalculateTotalSizes();
    return m_calculatedTotalFlexiblePxSize;
}

Vector2i ILayoutElement::GetTotalSize(LayoutSizeType sizeType) const
{
    if (sizeType == LayoutSizeType::Min) { return GetTotalMinSize(); }
    if (sizeType == LayoutSizeType::Preferred) { return GetTotalPreferredSize(); }
    if (sizeType == LayoutSizeType::Flexible) { return GetTotalFlexiblePxSize(); }
    return Vector2i::Zero;
}

void ILayoutElement::CalculateTotalSizes() const
{
    if (IsInvalid())
    {
        m_calculatedTotalMinSize        = CalculateTotalMinSize();
        m_calculatedTotalPreferredSize  = CalculateTotalPreferredSize();
        m_calculatedTotalFlexiblePxSize = CalculateTotalFlexiblePxSize();

        ILayoutElement *ncThis = const_cast<ILayoutElement*>(this);
        ncThis->SetInvalid(false);
    }
}
