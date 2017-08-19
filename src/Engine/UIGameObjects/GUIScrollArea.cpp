#include "Bang/GUIScrollArea.h"

#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"

GUIScrollArea::GUIScrollArea() noexcept : UIGameObject("GUIScrollArea")
{
    m_mask = new GUIMask();
    m_mask->SetParent(this);
    m_mask->SetName("GUIScrollArea_Mask");

    UIImageRenderer *quad = m_mask->AddComponent<UIImageRenderer>();
    quad->SetTint(Color::White);
    m_mask->SetDrawMask(false);

    m_childrenContainer = new UIGameObject("GUIScrollArea_ChildrenContainer");
    m_childrenContainer->SetParent(m_mask);
}

GUIScrollArea::~GUIScrollArea() noexcept
{
}

void GUIScrollArea::AddChild(UIGameObject *child) noexcept
{
    child->SetParent(m_childrenContainer);
}

void GUIScrollArea::SetMasking(bool masking)
{
    m_mask->SetMasking(masking);
}

void GUIScrollArea::SetScrolling(const Vector2i &scrollPx) noexcept
{
    m_scrollingPx = scrollPx;
    RefreshChildren();
}

void GUIScrollArea::SetScrollingX(int scrollPxX) noexcept
{
    SetScrolling( Vector2i(scrollPxX, GetScrolling().y) );
}

void GUIScrollArea::SetScrollingY(int scrollPxY) noexcept
{
    SetScrolling( Vector2i(GetScrolling().x, scrollPxY) );
}

bool GUIScrollArea::GetMasking() const { return m_mask->GetMasking(); }
const Vector2i &GUIScrollArea::GetScrolling() const noexcept
{
    return m_scrollingPx;
}

void GUIScrollArea::RefreshChildren()
{
    int marginLeft  =  GetScrolling().x;
    int marginRight = -GetScrolling().x;
    int marginTop   = -GetScrolling().y;
    int marginBot   =  GetScrolling().y;

    m_childrenContainer->rectTransform->SetMargins(marginLeft, marginTop,
                                                   marginRight, marginBot);
}
