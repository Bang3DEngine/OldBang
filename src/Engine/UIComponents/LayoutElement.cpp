#include "Bang/LayoutElement.h"

USING_NAMESPACE_BANG

LayoutElement::LayoutElement()
{

}

LayoutElement::~LayoutElement()
{

}

void Bang::LayoutElement::SetMinWidth(int minWidth)
{
    m_minWidth = minWidth;
}

void LayoutElement::SetMinHeight(int minHeight)
{
    m_minHeight = minHeight;
}

void LayoutElement::SetMinSize(const Vector2i &minSize)
{
    m_minWidth = minSize.x;
    m_minHeight = minSize.y;
}

void LayoutElement::SetPreferredWidth(int preferredWidth)
{
    m_preferredWidth = preferredWidth;
}

void LayoutElement::SetPreferredHeight(int preferredHeight)
{
    m_preferredHeight = preferredHeight;
}

void LayoutElement::SetPreferredSize(const Vector2i &preferredSize)
{
    SetPreferredWidth(preferredSize.x);
    SetPreferredHeight(preferredSize.y);
}

void LayoutElement::SetFlexibleWidth(int flexibleWidth)
{
    m_flexibleWidth = flexibleWidth;
}

void LayoutElement::SetFlexibleHeight(int flexibleHeight)
{
    m_flexibleHeight = flexibleHeight;
}

void LayoutElement::SetFlexibleSize(const Vector2i &flexibleSize)
{
    SetFlexibleWidth(flexibleSize.x);
    SetFlexibleHeight(flexibleSize.y);
}

int LayoutElement::GetMinWidth() const
{
    return m_minWidth;
}

int LayoutElement::GetMinHeight() const
{
    return m_minHeight;
}

Vector2i LayoutElement::GetMinSize() const
{
    return Vector2i(m_minWidth, m_minHeight);
}

int LayoutElement::GetPreferredWidth() const
{
    return m_preferredWidth;
}

int LayoutElement::GetPreferredHeight() const
{
    return m_preferredHeight;
}

Vector2i LayoutElement::GetPreferredSize() const
{
    return Vector2i(m_preferredWidth, m_preferredHeight);
}

float LayoutElement::GetFlexibleWidth() const
{
    return m_flexibleWidth;
}

float LayoutElement::GetFlexibleHeight() const
{
    return m_flexibleHeight;
}

Vector2 LayoutElement::GetFlexibleSize() const
{
    return Vector2(m_flexibleWidth, m_flexibleHeight);
}


