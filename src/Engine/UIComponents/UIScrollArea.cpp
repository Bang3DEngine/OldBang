#include "Bang/UIScrollArea.h"

#include "Bang/UIGameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutManager.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

UIScrollArea::UIScrollArea() noexcept
{
}

UIScrollArea::~UIScrollArea() noexcept
{
}

void UIScrollArea::OnUpdate()
{
    Component::OnUpdate();
    RetrieveReferences();
    UpdateChildrenMargins();
}

void UIScrollArea::SetMasking(bool masking)
{
    p_mask->SetMasking(masking);
}

void UIScrollArea::SetScrolling(const Vector2i &scrollPx) noexcept
{
    m_scrollingPx = scrollPx;
    UpdateChildrenMargins();
}

void UIScrollArea::SetScrollingX(int scrollPxX) noexcept
{
    SetScrolling( Vector2i(scrollPxX, GetScrolling().y) );
}

void UIScrollArea::SetScrollingY(int scrollPxY) noexcept
{
    SetScrolling( Vector2i(GetScrolling().x, scrollPxY) );
}

bool UIScrollArea::IsMasking() const { return p_mask->IsMasking(); }

UIGameObject *UIScrollArea::GetContainer() const
{
    return p_childrenContainer;
}
const Vector2i &UIScrollArea::GetScrolling() const noexcept
{
    return m_scrollingPx;
}

void UIScrollArea::UpdateChildrenMargins()
{
    int marginLeft  =  GetScrolling().x;
    int marginRight = -GetScrolling().x;
    int marginTop   = -GetScrolling().y;
    int marginBot   =  GetScrolling().y;

    p_childrenContainer->GetRectTransform()->SetMargins(marginLeft, marginTop,
                                                        marginRight, marginBot);
    UILayoutManager::Invalidate(p_childrenContainer->GetRectTransform());
}

UIGameObject *UIScrollArea::CreateGameObject()
{
    UIGameObject *go = GameObjectFactory::CreateUIGameObject(true);

    UIGameObject *mask = GameObjectFactory::CreateUIGameObject(true);
    mask->AddComponent<UIMask>();
    mask->SetName("Mask");
    mask->SetParent(go);

    UIGameObject *childrenCont = GameObjectFactory::CreateUIGameObject(true);
    childrenCont->SetName("ChildrenContainer");
    childrenCont->SetParent(mask);

    UIScrollArea *scrollArea = go->AddComponent<UIScrollArea>();
    scrollArea->InitGameObject();

    return go;
}

void UIScrollArea::RetrieveReferences()
{
    UIGameObject *go = SCAST<UIGameObject*>(GetGameObject()); ENSURE(go);
    p_mask = go->GetChild("Mask")->GetComponent<UIMask>();
    p_childrenContainer = SCAST<UIGameObject*>(p_mask->GetGameObject()->
                                               GetChild("ChildrenContainer"));
}

void UIScrollArea::InitGameObject()
{
    RetrieveReferences();
    p_mask->SetDrawMask(false);
    UIImageRenderer *quad = p_mask->GetGameObject()->AddComponent<UIImageRenderer>();
    quad->SetTint(Color::White);
    RetrieveReferences();
}
