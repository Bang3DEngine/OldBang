#include "Bang/UIScrollArea.h"

#include "Bang/GameObject.h"
#include "Bang/UIFrameLayout.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutManager.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

UIScrollArea::UIScrollArea()
{
}

UIScrollArea::~UIScrollArea()
{
}

void UIScrollArea::OnUpdate()
{
    Component::OnUpdate();
    UpdatePaddings();
}

void UIScrollArea::SetScrolling(const Vector2i &scrollPx)
{
    m_scrollingPx = scrollPx;
    UpdatePaddings();
}

void UIScrollArea::SetScrollingX(int scrollPxX)
{
    SetScrolling( Vector2i(scrollPxX, GetScrolling().y) );
}

void UIScrollArea::SetScrollingY(int scrollPxY)
{
    SetScrolling( Vector2i(GetScrolling().x, scrollPxY) );
}

UIMask *UIScrollArea::GetMask() const
{
    return p_mask;
}

GameObject *UIScrollArea::GetContainer() const
{
    return p_childrenContainer;
}
const Vector2i &UIScrollArea::GetScrolling() const
{
    return m_scrollingPx;
}

void UIScrollArea::UpdatePaddings()
{
    int paddingLeft  =  GetScrolling().x;
    int paddingRight = -GetScrolling().x;
    int paddingTop   = -GetScrolling().y;
    int paddingBot   =  GetScrolling().y;

    UIFrameLayout *fl = gameObject->GetComponent<UIFrameLayout>();
    fl->SetPaddings(paddingLeft, paddingBot, paddingRight, paddingTop);
}

UIScrollArea* UIScrollArea::CreateInto(GameObject *go)
{
    UIScrollArea *scrollArea = go->AddComponent<UIScrollArea>();
    go->AddComponent<UIFrameLayout>();

    UIMask *mask = go->AddComponent<UIMask>();
    mask->SetDrawMask(false);

    UIImageRenderer *quad = go->AddComponent<UIImageRenderer>();
    quad->SetTint(Color::Yellow);

    GameObject *childrenCont = GameObjectFactory::CreateUIGameObject();
    childrenCont->SetName("ChildrenContainer");
    go->AddChild(childrenCont);

    scrollArea->p_mask = mask;
    scrollArea->p_childrenContainer = childrenCont;

    return scrollArea;
}
