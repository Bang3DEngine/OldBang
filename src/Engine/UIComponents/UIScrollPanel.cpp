#include "Bang/UIScrollPanel.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/GameObject.h"
#include "Bang/UIScrollBar.h"
#include "Bang/UIFocusTaker.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIFocusTaker.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutManager.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

UIScrollPanel::UIScrollPanel()
{
}

UIScrollPanel::~UIScrollPanel()
{
}

void UIScrollPanel::OnUpdate()
{
    Component::OnUpdate();

    Vector2i contentSize( GetContentSize() );
    if (contentSize != Vector2i::Zero)
    {
        Vector2i containerSize( GetContainerSize() );
        Vector2 sizeProp = Vector2(containerSize) / Vector2(contentSize);
        sizeProp = Vector2::Clamp(sizeProp, Vector2(0.1f), Vector2::One);

        const bool isVertical = GetScrollBar()->IsVertical();
        GetScrollBar()->SetLengthPercent(isVertical ? sizeProp.y : sizeProp.x);

        Vector2 scrollingPercent = Vector2(GetScrollBar()->GetScrollingPercent());
        scrollingPercent = Vector2::Clamp(scrollingPercent, Vector2::Zero,
                                          Vector2::One);

        Vector2i scrollMaxAmount(contentSize - containerSize);
        scrollMaxAmount = Vector2i::Max(scrollMaxAmount, Vector2i::One);

        Vector2i scrolling( Vector2::Round( scrollingPercent *
                                            Vector2(scrollMaxAmount) ) );

        // MouseWheel scrolling
        if (gameObject->GetComponent<UIFocusTaker>()->HasFocus())
        {
            int mouseWheelPx = Input::GetMouseWheel() * WheelScrollSpeedPx;
            scrolling -= Vector2i(0, mouseWheelPx);
        }

        // Apply scrolling
        scrolling  = Vector2i::Clamp(scrolling, Vector2i::Zero, scrollMaxAmount);
        scrolling *= (isVertical ? Vector2i::Up : Vector2i::Right);
        GetScrollArea()->SetScrolling( scrolling );

        scrollingPercent = Vector2(scrolling) / Vector2(scrollMaxAmount);
        GetScrollBar()->SetScrollingPercent( isVertical ? scrollingPercent.y :
                                                          scrollingPercent.x );
        // Debug_Peek(sizeProp);
        // Debug_Peek(scrolling);
        // Debug_Peek(contentSize);
        // Debug_Peek(containerSize);
        // Debug_Peek(scrollMaxAmount);
        // Debug_Peek(GetScrollArea()->GetScrolling());
        // Debug_Peek(GetScrollBar()->GetScrollingPercent());
        // Debug_Log("=====================");
    }

    if (Input::GetKeyDown(Key::E)) { GetScrollArea()->SetScrollingY( GetScrollArea()->GetScrolling().y - 10); }
    if (Input::GetKeyDown(Key::R)) { GetScrollArea()->SetScrollingY( GetScrollArea()->GetScrolling().y + 10); }
    /*
    Debug_Peek(contentSize);
    Debug_Peek(GetScrollArea()->GetScrolling());
    */
}

void UIScrollPanel::SetScrolling(const Vector2i &scrolling)
{
    Vector2i contentSize = GetContentSize();
    SetScrollingPercent( Vector2(scrolling) / Vector2(contentSize) );
}

void UIScrollPanel::SetScrollingPercent(const Vector2 &scrollPerc)
{
    Vector2i contentSize = GetContentSize();
    GetScrollArea()->SetScrolling(
                Vector2i( Vector2::Round(scrollPerc * Vector2(contentSize))) );

    bool isVertical = GetScrollBar()->IsVertical();
    GetScrollBar()->SetScrollingPercent(isVertical ? scrollPerc.x : scrollPerc.y);
}

Vector2i UIScrollPanel::GetContentSize() const
{
    return UILayoutManager::GetPreferredSize(GetContainer());
}

Vector2i UIScrollPanel::GetContainerSize() const
{
    return GetContainer()->GetComponent<RectTransform>()->
           GetScreenSpaceRectPx().GetSize();
}

GameObject *UIScrollPanel::GetContainer() const
{
    return GetScrollArea()->GetContainer();
}

UIScrollArea *UIScrollPanel::GetScrollArea() const
{
    return p_scrollArea;
}

UIScrollBar *UIScrollPanel::GetScrollBar() const
{
    return p_scrollBar;
}

UIScrollPanel *UIScrollPanel::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);
    REQUIRE_COMPONENT(go, UIFocusTaker);

    UIHorizontalLayout *hl = go->AddComponent<UIHorizontalLayout>();

    UIScrollPanel *scrollPanel = go->AddComponent<UIScrollPanel>();

    UIScrollArea *scrollArea = GameObjectFactory::CreateUIScrollArea();

    UILayoutElement *scrollAreaLE = scrollArea->gameObject->
                                    AddComponent<UILayoutElement>();
    scrollAreaLE->SetFlexibleSize( Vector2(1) );

    UIScrollBar *scrollBar = GameObjectFactory::CreateUIScrollBar();

    go->AddChild(scrollArea->gameObject);
    go->AddChild(scrollBar->gameObject);

    scrollPanel->p_scrollArea = scrollArea;
    scrollPanel->p_scrollBar  = scrollBar;

    return scrollPanel;
}

