#include "Bang/UIScrollArea.h"

#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
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

void UIScrollArea::SetContainedGameObject(GameObject *go)
{
    if (GetContainedGameObject() != go)
    {
        if (GetContainedGameObject())
        {
            GetContainedGameObject()->SetParent(nullptr);
        }

        p_containedGo = go;
        if (GetContainedGameObject())
        {
            GetContainedGameObject()->SetParent( GetContainer() );
        }
    }
}

UIMask *UIScrollArea::GetMask() const { return p_mask; }
GameObject *UIScrollArea::GetContainer() const { return p_container; }
GameObject *UIScrollArea::GetContainedGameObject() const { return p_containedGo; }
UIImageRenderer *UIScrollArea::GetBackground() const { return p_bg; }
const Vector2i &UIScrollArea::GetScrolling() const { return m_scrollingPx; }

void UIScrollArea::UpdatePaddings()
{
    int paddingLeft  =  GetScrolling().x;
    int paddingRight = -GetScrolling().x;
    int paddingTop   =  GetScrolling().y;
    int paddingBot   = -GetScrolling().y;

    RectTransform *rt = GetContainer()->GetRectTransform();
    rt->SetMargins(paddingLeft, paddingBot, paddingRight, paddingTop);
}

UIScrollArea* UIScrollArea::CreateInto(GameObject *go)
{
    UIScrollArea *scrollArea = go->AddComponent<UIScrollArea>();

    UIImageRenderer *bg = go->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::White);

    UIMask *mask = go->AddComponent<UIMask>();
    go->AddComponent<UIImageRenderer>();
    mask->SetDrawMask(false);

    GameObject *childrenCont = GameObjectFactory::CreateUIGameObject();
    childrenCont->SetName("ScrollAreaChildrenContainer");

    scrollArea->p_bg = bg;
    scrollArea->p_mask = mask;
    scrollArea->p_container = childrenCont;

    go->SetAsChild(childrenCont);

    return scrollArea;
}
