#include "Bang/UIScrollArea.h"

#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutManager.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
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

    GetGameObject()->GetComponent<RectTransform>()->Invalidate();
}

void UIScrollArea::SetScrolling(const Vector2i &scrollPx)
{
    if (m_scrollingPx != scrollPx)
    {
        m_scrollingPx = scrollPx;
        UpdatePaddings();
    }
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

UIImageRenderer *UIScrollArea::GetBackground() const
{
    return p_bg;
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

    UIGroupLayout *gl = GetGameObject()->GetComponent<UIGroupLayout>();
    gl->SetPaddings(paddingLeft, paddingBot, paddingRight, paddingTop);
}

UIScrollArea* UIScrollArea::CreateInto(GameObject *go)
{
    UIScrollArea *scrollArea = go->AddComponent<UIScrollArea>();
    UIVerticalLayout *vl = go->AddComponent<UIVerticalLayout>();
    vl->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    vl->SetChildrenVerticalAlignment(VerticalAlignment::Top);

    UIImageRenderer *bg = go->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::White);

    UIMask *mask = go->AddComponent<UIMask>();
    UIImageRenderer *maskQuad = go->AddComponent<UIImageRenderer>();
    mask->SetDrawMask(false);

    GameObject *childrenCont = GameObjectFactory::CreateUIGameObject();
    childrenCont->SetName("ChildrenCont");

    scrollArea->p_bg = bg;
    scrollArea->p_mask = mask;
    scrollArea->p_childrenContainer = childrenCont;

    go->AddChild(childrenCont);

    return scrollArea;
}
