#include "Bang/UIFrameLayout.h"

#include "Bang/List.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

UIFrameLayout::UIFrameLayout()
{

}

UIFrameLayout::~UIFrameLayout()
{

}

void UIFrameLayout::SetPaddings(int padding)
{
    SetPaddings(padding, padding, padding, padding);
}

void UIFrameLayout::SetPaddings(int paddingLeft, int paddingBot,
                                int paddingRight, int paddingTop)
{
    if (m_paddingLeftBot.x != paddingLeft   ||
        m_paddingLeftBot.y != paddingBot    ||
        m_paddingRightTop.x != paddingRight ||
        m_paddingRightTop.y != paddingTop)
    {
        m_paddingLeftBot  = Vector2i(paddingLeft,  paddingBot);
        m_paddingRightTop = Vector2i(paddingRight, paddingTop);
        UILayoutManager::InvalidateLayoutUpwards(gameObject);
    }
}

int UIFrameLayout::GetPaddingLeft() const
{
    return GetPaddingLeftBot().x;
}

int UIFrameLayout::GetPaddingBot() const
{
    return GetPaddingLeftBot().y;
}

int UIFrameLayout::GetPaddingRight() const
{
    return GetPaddingRightTop().x;
}

int UIFrameLayout::GetPaddingTop() const
{
    return GetPaddingRightTop().y;
}

const Vector2i &UIFrameLayout::GetPaddingLeftBot() const
{
    return m_paddingLeftBot;
}

const Vector2i &UIFrameLayout::GetPaddingRightTop() const
{
    return m_paddingRightTop;
}

Vector2i UIFrameLayout::GetPaddingSize() const
{
    return GetPaddingLeftBot() + GetPaddingRightTop();
}

void UIFrameLayout::ApplyLayoutToChildren()
{
}

Vector2i UIFrameLayout::CalculateTotalMinSize() const
{
    Vector2i result = GetPaddingSize();
    ILayoutElement* layoutElm =
            gameObject->GetComponentInChildrenOnly<ILayoutElement>(false);
    if (layoutElm)
    {
        result += layoutElm->GetTotalMinSize();
    }
    return result;
}

Vector2i UIFrameLayout::CalculateTotalPreferredSize() const
{
    Vector2i result = GetPaddingSize();
    ILayoutElement* layoutElm =
            gameObject->GetComponentInChildrenOnly<ILayoutElement>(false);
    if (layoutElm)
    {
        result += layoutElm->GetTotalPreferredSize();
    }
    return result;
}

Vector2i UIFrameLayout::CalculateTotalFlexiblePxSize() const
{
    Vector2i result = GetPaddingSize();
    ILayoutElement* layoutElm =
            gameObject->GetComponentInChildrenOnly<ILayoutElement>(false);
    if (layoutElm)
    {
        result += layoutElm->GetTotalFlexiblePxSize();
    }
    return result;
}
