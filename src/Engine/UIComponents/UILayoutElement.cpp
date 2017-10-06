#include "Bang/UILayoutElement.h"

USING_NAMESPACE_BANG

UILayoutElement::UILayoutElement()
{
    SetLayoutPriority(1);
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

void UILayoutElement::SetFlexibleWidth(float flexibleWidth)
{
    m_flexibleSize.x = flexibleWidth;
}

void UILayoutElement::SetFlexibleHeight(float flexibleHeight)
{
    m_flexibleSize.y = flexibleHeight;
}

void UILayoutElement::SetFlexibleSize(const Vector2 &flexibleSize)
{
    m_flexibleSize = flexibleSize;
}


int UILayoutElement::GetMinWidth() const
{
    return GetMinSize().x;
}

int UILayoutElement::GetMinHeight() const
{
    return GetMinSize().y;
}

const Vector2i& UILayoutElement::GetMinSize() const
{
    return m_minSize;
}

int UILayoutElement::GetPreferredWidth() const
{
    return GetPreferredSize().x;
}

int UILayoutElement::GetPreferredHeight() const
{
    return GetPreferredSize().y;
}

const Vector2i& UILayoutElement::GetPreferredSize() const
{
    return m_preferredSize;
}

float UILayoutElement::GetFlexibleWidth() const
{
    return GetFlexibleSize().x;
}

float UILayoutElement::GetFlexibleHeight() const
{
    return GetFlexibleSize().y;
}

const Vector2 &UILayoutElement::GetFlexibleSize() const
{
    return m_flexibleSize;
}

Vector2 UILayoutElement::_GetFlexibleSize() const
{
    return GetFlexibleSize();
}

Vector2i UILayoutElement::_GetMinSize() const
{
    return GetMinSize();
}

Vector2i UILayoutElement::_GetPreferredSize() const
{
    return GetPreferredSize();
}
