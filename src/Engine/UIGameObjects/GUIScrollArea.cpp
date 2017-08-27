#include "Bang/GUIScrollArea.h"

#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"

GUIScrollArea::GUIScrollArea() noexcept
{
}

GUIScrollArea::~GUIScrollArea() noexcept
{
}

void GUIScrollArea::OnUpdate()
{
    UIComponent::OnUpdate();
    RetrieveReferences();
    UpdateChildrenMargins();
}

void GUIScrollArea::SetMasking(bool masking)
{
    p_mask->SetMasking(masking);
}

void GUIScrollArea::SetScrolling(const Vector2i &scrollPx) noexcept
{
    m_scrollingPx = scrollPx;
    UpdateChildrenMargins();
}

void GUIScrollArea::SetScrollingX(int scrollPxX) noexcept
{
    SetScrolling( Vector2i(scrollPxX, GetScrolling().y) );
}

void GUIScrollArea::SetScrollingY(int scrollPxY) noexcept
{
    SetScrolling( Vector2i(GetScrolling().x, scrollPxY) );
}

bool GUIScrollArea::GetMasking() const { return p_mask->GetMasking(); }

UIGameObject *GUIScrollArea::GetContainer() const
{
    return p_childrenContainer;
}
const Vector2i &GUIScrollArea::GetScrolling() const noexcept
{
    return m_scrollingPx;
}

void GUIScrollArea::UpdateChildrenMargins()
{
    int marginLeft  =  GetScrolling().x;
    int marginRight = -GetScrolling().x;
    int marginTop   = -GetScrolling().y;
    int marginBot   =  GetScrolling().y;

    p_childrenContainer->rectTransform->SetMargins(marginLeft, marginTop,
                                                   marginRight, marginBot);
}

UIGameObject *GUIScrollArea::CreateGameObject()
{
    UIGameObject *go = new UIGameObject();

    GUIMask *mask = new GUIMask();
    mask->SetName("Mask");
    mask->SetParent(go);

    UIGameObject *childrenCont = new UIGameObject("ChildrenContainer");
    childrenCont->SetParent(mask);

    GUIScrollArea *scrollArea = go->AddComponent<GUIScrollArea>();
    scrollArea->InitGameObject();

    return go;
}

void GUIScrollArea::RetrieveReferences()
{
    UIGameObject *go = GetGameObject(); ENSURE(go);
    p_mask = SCAST<GUIMask*>(go->GetChild("Mask"));
    p_childrenContainer = SCAST<UIGameObject*>(p_mask->GetChild("ChildrenContainer"));
}

void GUIScrollArea::InitGameObject()
{
    RetrieveReferences();
    p_mask->SetDrawMask(false);
    UIImageRenderer *quad = p_mask->AddComponent<UIImageRenderer>();
    quad->SetTint(Color::White);
}
