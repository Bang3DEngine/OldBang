#include "Bang/UIScrollArea.h"

#include "Bang/GameObject.h"
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

GameObject *UIScrollArea::GetContainer() const
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

    p_childrenContainer->GetComponent<RectTransform>()->
                          SetMargins(marginLeft, marginTop,
                                     marginRight, marginBot);
    UILayoutManager::Invalidate(p_childrenContainer->
                                GetComponent<RectTransform>());
}

GameObject *UIScrollArea::CreateGameObject()
{
    GameObject *go = GameObjectFactory::CreateUIGameObject(true);

    GameObject *mask = GameObjectFactory::CreateUIGameObject(true);
    mask->AddComponent<UIMask>();
    mask->SetName("Mask");
    mask->SetParent(go);

    GameObject *childrenCont = GameObjectFactory::CreateUIGameObject(true);
    childrenCont->SetName("ChildrenContainer");
    childrenCont->SetParent(mask);

    UIScrollArea *scrollArea = go->AddComponent<UIScrollArea>();
    scrollArea->InitGameObject();

    return go;
}

void UIScrollArea::RetrieveReferences()
{
    p_mask = gameObject->GetChild("Mask")->GetComponent<UIMask>();
    p_childrenContainer = p_mask->gameObject->GetChild("ChildrenContainer");
}

void UIScrollArea::InitGameObject()
{
    RetrieveReferences();
    p_mask->SetDrawMask(false);
    UIImageRenderer *quad = p_mask->GetGameObject()->AddComponent<UIImageRenderer>();
    quad->SetTint(Color::White);
    RetrieveReferences();
}
