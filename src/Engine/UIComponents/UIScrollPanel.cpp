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

    Axis axis = GetScrollBar()->GetScrollAxis();
    int contentSize = GetContentSize().GetAxis(axis);
    int containerSize = GetContainerSize().GetAxis(axis);

    // Handle vertical/horizontal show mode
    HandleScrollShowMode(contentSize, containerSize);

    // Set containedGo anchors according to the current scrolling and scroll
    // area margins to leave space for the scroll bar
    HandleScrollAreaRectTransform();

    // Handle scroll percent and scroll bar
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
        // Reestablish bar length
        GetScrollBar()->SetLength( GetContainerSize().y );
    }
}

void UIScrollPanel::SetScrollBarSide(VerticalSide side)
{
    if (GetVerticalScrollBarSide() != side)
    {
        GetScrollBar()->SetSide(side == VerticalSide::Top ? Side::Top :
                                                            Side::Bot);
        m_verticalScrollBarSide = side;
    }
}

void UIScrollPanel::SetScrollBarSide(HorizontalSide side)
{
    if (GetHorizontalScrollBarSide() != side)
    {
        GetScrollBar()->SetSide(side == HorizontalSide::Left ? Side::Left :
                                                               Side::Right);
        m_horizontalScrollBarSide = side;
    }
}

void UIScrollPanel::SetVerticalShowScrollMode(ShowScrollMode showScrollMode)
{
    if (GetVerticalShowScrollMode() != showScrollMode)
    {
        m_verticalShowScrollMode = showScrollMode;
    }
}

void UIScrollPanel::SetHorizontalShowScrollMode(ShowScrollMode showScrollMode)
{
    if (GetHorizontalShowScrollMode() != showScrollMode)
    {
        m_horizontalShowScrollMode = showScrollMode;
    }
}

void UIScrollPanel::SetScrolling(const Vector2i &scrolling)
{
    Vector2 contentSize = GetContentSize();
    contentSize = Vector2::Max(contentSize, Vector2::One);
    SetScrollingPercent( Vector2(scrolling) / contentSize );
}

void UIScrollPanel::SetScrollingPercent(const Vector2 &scrollPerc)
{
    Vector2 contentSize = GetContentSize();
    GetScrollArea()->SetScrolling(
                Vector2i( Vector2::Round(scrollPerc * contentSize)) );

    Axis axis = GetScrollBar()->GetScrollAxis();
    GetScrollBar()->SetScrollingPercent( scrollPerc.GetAxis(axis) );
}

VerticalSide UIScrollPanel::GetVerticalScrollBarSide() const
{
    return m_verticalScrollBarSide;
}

HorizontalSide UIScrollPanel::GetHorizontalScrollBarSide() const
{
    return m_horizontalScrollBarSide;
}

ShowScrollMode UIScrollPanel::GetVerticalShowScrollMode() const
{
    return m_verticalShowScrollMode;
}

ShowScrollMode UIScrollPanel::GetHorizontalShowScrollMode() const
{
    return m_horizontalShowScrollMode;
}

Vector2 UIScrollPanel::GetContentSize() const
{
    GameObject *containedGo = GetScrollArea()->GetContainedGameObject();
    return containedGo ? containedGo->GetComponent<RectTransform>()->
                            GetScreenSpaceRectPx().GetSize() : Vector2::Zero;
}

Vector2 UIScrollPanel::GetContainerSize() const
{
    return GetScrollArea()->GetContainer()->GetParent()->
           GetComponent<RectTransform>()->GetScreenSpaceRectPx().GetSize();
}

UIScrollArea *UIScrollPanel::GetScrollArea() const { return p_scrollArea; }
UIScrollBar *UIScrollPanel::GetScrollBar() const { return p_scrollBar; }

void UIScrollPanel::HandleScrollAreaRectTransform()
{
    RectTransform *scrollAreaRT =
            GetScrollArea()->GetGameObject()->GetComponent<RectTransform>();

    int scrollBarThickness = (GetScrollBar()->GetGameObject()->IsEnabled() ?
                                  GetScrollBar()->GetThickness() : 0 );
    if (GetHorizontalScrollBarSide() == HorizontalSide::Right)
    {
        scrollAreaRT->SetMarginRight(scrollBarThickness);
    }
    else
    {
        scrollAreaRT->SetMarginLeft(scrollBarThickness);
    }

    GameObject *containedGo = GetScrollArea()->GetContainedGameObject();
    if (containedGo)
    {
        RectTransform *containedGoRT = containedGo->GetComponent<RectTransform>();
        if (GetScrollBar()->GetScrollAxis() == Axis::Vertical)
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
}

void UIScrollPanel::HandleScrollShowMode(int contentSize, int containerSize)
{
    bool showVertical = false;
    switch (GetVerticalShowScrollMode())
    {
        case ShowScrollMode::Never: showVertical = false; break;
        case ShowScrollMode::WhenNeeded:
            showVertical = (contentSize > containerSize); break;
        case ShowScrollMode::Always: showVertical = true; break;
    }

    bool showHorizontal = false;
    switch (GetHorizontalShowScrollMode())
    {
        case ShowScrollMode::Never: showHorizontal = false; break;
        case ShowScrollMode::WhenNeeded:
            showHorizontal = (contentSize > containerSize); break;
        case ShowScrollMode::Always: showHorizontal = true; break;
    }

    GetScrollBar()->GetGameObject()->SetEnabled(showVertical);
}


UIScrollPanel *UIScrollPanel::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UIScrollPanel *scrollPanel = go->AddComponent<UIScrollPanel>();

    UIScrollArea *scrollArea = GameObjectFactory::CreateUIScrollArea();
    UIScrollBar *scrollBar = GameObjectFactory::CreateUIScrollBar();

    go->SetAsChild(scrollArea->GetGameObject());
    go->SetAsChild(scrollBar->GetGameObject());

    scrollPanel->p_scrollArea = scrollArea;
    scrollPanel->p_scrollBar  = scrollBar;

    scrollPanel->SetVerticalShowScrollMode(ShowScrollMode::Always);
    scrollPanel->SetHorizontalShowScrollMode(ShowScrollMode::Always);
    scrollPanel->SetScrollBarSide(VerticalSide::Top);
    scrollPanel->SetScrollBarSide(HorizontalSide::Right);

    return scrollPanel;
}

