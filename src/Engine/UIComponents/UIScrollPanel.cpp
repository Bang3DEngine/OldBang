#include "Bang/UIScrollPanel.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/Color.h"
#include "Bang/GameObject.h"
#include "Bang/UIScrollBar.h"
#include "Bang/UIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"

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

    RectTransform *scrollAreaRT =
            GetScrollArea()->GetGameObject()->GetComponent<RectTransform>();
    scrollAreaRT->SetMarginRight(GetScrollBar()->GetThickness());

    GameObject *containedGo = GetScrollArea()->GetContainedGameObject();
    if (containedGo)
    {
        RectTransform *containedGoRT = containedGo->GetComponent<RectTransform>();
        if (GetScrollBar()->GetAxis() == Axis::Vertical)
        {
            containedGoRT->SetAnchorX( Vector2(-1,1) );
            containedGoRT->SetAnchorY( Vector2(1) );
        }
        else
        {
            containedGoRT->SetAnchorX( Vector2(-1) );
            containedGoRT->SetAnchorY( Vector2(-1,1) );
        }
    }

    Axis axis = GetScrollBar()->GetAxis();
    int contentSize = GetContentSize().GetAxis(axis);
    int containerSize = GetContainerSize().GetAxis(axis);
    if (contentSize > containerSize)
    {
        // Set bar length
        Vector2 sizeProp = Vector2(containerSize) / Vector2(contentSize);
        sizeProp = Vector2::Clamp(sizeProp, Vector2(0.1f), Vector2::One);
        GetScrollBar()->SetLengthPercent( sizeProp.GetAxis(axis) );

        // Get scrolling percent from the ScrollBar
        float scrollingPercent = GetScrollBar()->GetScrollingPercent();
        scrollingPercent = Math::Clamp(scrollingPercent, 0.0f, 1.0f);

        // MouseWheel scrolling
        if (GetGameObject()->GetComponent<RectTransform>()->IsMouseOver())
        {
            int mouseWheelPx = Input::GetMouseWheel() * WheelScrollSpeedPx;
            float mouseWheelPercent = float(mouseWheelPx) / contentSize;
            scrollingPercent -= mouseWheelPercent;
        }
        scrollingPercent = Math::Clamp(scrollingPercent, 0, 1);

        // Apply scrollings
        int scrollMaxAmount = (contentSize - containerSize);
        Vector2i scrolling(scrollingPercent * scrollMaxAmount);
        scrolling *= (axis == Axis::Vertical ? Vector2i::Up : Vector2i::Right);
        GetScrollArea()->SetScrolling( scrolling );
        GetScrollBar()->SetScrollingPercent( scrollingPercent );
    }
    else
    {
        GetScrollBar()->SetLength( GetContainerSize().y );
    }
}

void UIScrollPanel::SetScrolling(const Vector2i &scrolling)
{
    Vector2 contentSize = GetContentSize();
    SetScrollingPercent( Vector2(scrolling) / contentSize );
}

void UIScrollPanel::SetScrollingPercent(const Vector2 &scrollPerc)
{
    Vector2 contentSize = GetContentSize();
    GetScrollArea()->SetScrolling(
                Vector2i( Vector2::Round(scrollPerc * contentSize)) );

    Axis axis = GetScrollBar()->GetAxis();
    GetScrollBar()->SetScrollingPercent( scrollPerc.GetAxis(axis) );
}

Vector2 UIScrollPanel::GetContentSize() const
{
    return GetScrollArea()->GetContainedGameObject()->
           GetComponent<RectTransform>()->GetScreenSpaceRectPx().GetSize();
}

Vector2 UIScrollPanel::GetContainerSize() const
{
    return GetScrollArea()->GetContainer()->GetParent()->
           GetComponent<RectTransform>()->GetScreenSpaceRectPx().GetSize();
}

UIScrollArea *UIScrollPanel::GetScrollArea() const { return p_scrollArea; }
UIScrollBar *UIScrollPanel::GetScrollBar() const { return p_scrollBar; }

UIScrollPanel *UIScrollPanel::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UIScrollPanel *scrollPanel = go->AddComponent<UIScrollPanel>();

    UIScrollArea *scrollArea = GameObjectFactory::CreateUIScrollArea();
    UIScrollBar *scrollBar = GameObjectFactory::CreateUIScrollBar();

    go->AddChild(scrollArea->GetGameObject());
    go->AddChild(scrollBar->GetGameObject());

    scrollPanel->p_scrollArea = scrollArea;
    scrollPanel->p_scrollBar  = scrollBar;

    return scrollPanel;
}

