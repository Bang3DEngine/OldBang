#include "Bang/UILayoutElement.h"

USING_NAMESPACE_BANG

UILayoutElement::UILayoutElement()
{

}

UILayoutElement::~UILayoutElement()
{

}

void UILayoutElement::SetMinWidth(int minWidth)
{
    m_minSize.x = minWidth;
}

void UILayoutElement::SetMinHeight(int minHeight)
{
    m_minSize.y = minHeight;
}

void UILayoutElement::SetMinSize(const Vector2i &minSize)
{
    m_minSize = minSize;
}

void UILayoutElement::SetPreferredWidth(int preferredWidth)
{
    m_preferredSize.x = preferredWidth;
}

void UILayoutElement::SetPreferredHeight(int preferredHeight)
{
    m_preferredSize.y = preferredHeight;
}

void UILayoutElement::SetPreferredSize(const Vector2i &preferredSize)
{
    m_preferredSize = preferredSize;
}

void UILayoutElement::SetFlexibleWidth(int flexibleWidth)
{
    m_flexibleSize.x = flexibleWidth;
}

void UILayoutElement::SetFlexibleHeight(int flexibleHeight)
{
    m_flexibleSize.y = flexibleHeight;
}

void UILayoutElement::SetFlexibleSize(const Vector2 &flexibleSize)
{
    m_flexibleSize = flexibleSize;
}


int UILayoutElement::GetMinWidth() const
{
    return m_minSize.x;
}

int UILayoutElement::GetMinHeight() const
{
    return m_minSize.y;
}

const Vector2i& UILayoutElement::GetMinSize() const
{
    return m_minSize;
}

int UILayoutElement::GetPreferredWidth() const
{
    return m_preferredSize.x;
}

int UILayoutElement::GetPreferredHeight() const
{
    return m_preferredSize.y;
}

const Vector2i& UILayoutElement::GetPreferredSize() const
{
    return m_preferredSize;
}

float UILayoutElement::GetFlexibleWidth() const
{
    return m_flexibleSize.x;
}

float UILayoutElement::GetFlexibleHeight() const
{
    return m_flexibleSize.y;
}

const Vector2& UILayoutElement::GetFlexibleSize() const
{
    return m_flexibleSize;
}

int UILayoutElement::GetFlexiblePxWidth() const
{
    return GetFlexiblePxSize().x;
}

int UILayoutElement::GetFlexiblePxHeight() const
{
    return GetFlexiblePxSize().y;
}

Vector2i UILayoutElement::GetFlexiblePxSize() const
{
    return Vector2i(Vector2(GetPreferredSize()) * GetFlexibleSize());
}

Vector2i UILayoutElement::CalculateTotalFlexiblePxSize() const
{
    return GetFlexiblePxSize();
}

Vector2i UILayoutElement::CalculateTotalPreferredSize() const
{
    return GetPreferredSize();
}

Vector2i UILayoutElement::CalculateTotalMinSize() const
{
    return GetMinSize();
}
