#include "Bang/UIScrollPanel.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/Color.h"
#include "Bang/GameObject.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIScrollBar.h"
#include "Bang/UIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"

USING_NAMESPACE_BANG

const float UIScrollPanel::WheelScrollSpeedPx = 50.0f;

UIScrollPanel::UIScrollPanel()
{
}

UIScrollPanel::~UIScrollPanel()
{
}

void UIScrollPanel::UpdateScrollUI()
{
    Vector2 contentSize = GetContentSize();
    Vector2 containerSize = GetContainerSize();

    // Handle vertical/horizontal show mode
    HandleScrollShowMode(contentSize, containerSize);

    // Set containedGo anchors according to the current scrolling and scroll
    // area margins to leave space for the scroll bar
    HandleScrollAreaRectTransform();

    // Handle scroll percent and scroll bar
    if (contentSize.x > containerSize.x || contentSize.y > containerSize.y)
    {
        // Set bar length
        Vector2 sizeProp = Vector2(containerSize) / Vector2(contentSize);
        sizeProp = Vector2::Clamp(sizeProp, Vector2(0.1f), Vector2::One);
        GetHorizontalScrollBar()->SetLengthPercent( sizeProp.x );
        GetVerticalScrollBar()->SetLengthPercent( sizeProp.y );

        // Get scrolling percent from the scrollBar
        Vector2 scrollingPercent(GetHorizontalScrollBar()->GetScrollingPercent(),
                                 GetVerticalScrollBar()->GetScrollingPercent());
        scrollingPercent = Vector2::Clamp(scrollingPercent, Vector2::Zero,
                                                            Vector2::One);

        // MouseWheel scrolling
        if (GetGameObject()->GetComponent<RectTransform>()->IsMouseOver())
        {
            Vector2i mouseWheelPx(Input::GetMouseWheel() * WheelScrollSpeedPx);
            Vector2 mouseWheelPercent = Vector2(mouseWheelPx) / contentSize;
            scrollingPercent -= mouseWheelPercent;
        }
        scrollingPercent = Vector2::Clamp(scrollingPercent, Vector2::Zero,
                                                            Vector2::One);

        // Apply scrollings
        Vector2i scrollEnabledMask(IsHorizontalScrollEnabled() ? 1 : 0,
                                     IsVerticalScrollEnabled() ? 1 : 0);
        scrollingPercent *= Vector2(scrollEnabledMask);

        Vector2 scrollMaxAmount = (contentSize - containerSize);
        Vector2i scrolling(scrollingPercent * scrollMaxAmount);

        GetScrollArea()->SetScrolling( scrolling * Vector2i(-1, 1) );
        GetHorizontalScrollBar()->SetScrollingPercent( scrollingPercent.x );
        GetVerticalScrollBar()->SetScrollingPercent( scrollingPercent.y );
    }

    // If no scroll needed, reestablish bar lengths
    if (contentSize.x < containerSize.x)
    {
        GetHorizontalScrollBar()->SetLengthPercent(1.0f);
    }
    if (contentSize.y < containerSize.y)
    {
        GetVerticalScrollBar()->SetLengthPercent(1.0f);
    }
}

void UIScrollPanel::OnUpdate()
{
    Component::OnUpdate();

    UpdateScrollUI();
}

void UIScrollPanel::SetVerticalScrollBarSide(HorizontalSide side)
{
    if (GetVerticalScrollBarSide() != side)
    {
        GetVerticalScrollBar()->SetSide(side == HorizontalSide::Left ? Side::Left :
                                                                       Side::Right);
        m_verticalScrollBarSide = side;
    }
}

void UIScrollPanel::SetHorizontalScrollBarSide(VerticalSide side)
{
    if (GetHorizontalScrollBarSide() != side)
    {
        GetHorizontalScrollBar()->SetSide(side == VerticalSide::Top ? Side::Top :
                                                                      Side::Bot);
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

void UIScrollPanel::SetVerticalScrollEnabled(bool enabled)
{ m_verticalScrollEnabled = enabled; }
void UIScrollPanel::SetHorizontalScrollEnabled(bool enabled)
{ m_horizontalScrollEnabled = enabled; }

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

    GetHorizontalScrollBar()->SetScrollingPercent( scrollPerc.x );
    GetVerticalScrollBar()->SetScrollingPercent( scrollPerc.y );
}

HorizontalSide UIScrollPanel::GetVerticalScrollBarSide() const
{ return m_verticalScrollBarSide; }
VerticalSide UIScrollPanel::GetHorizontalScrollBarSide() const
{ return m_horizontalScrollBarSide; }
ShowScrollMode UIScrollPanel::GetVerticalShowScrollMode() const
{ return m_verticalShowScrollMode; }
ShowScrollMode UIScrollPanel::GetHorizontalShowScrollMode() const
{ return m_horizontalShowScrollMode; }
bool UIScrollPanel::IsVerticalScrollEnabled() const
{ return m_verticalScrollEnabled; }
bool UIScrollPanel::IsHorizontalScrollEnabled() const
{ return m_horizontalScrollEnabled; }

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
UIScrollBar *UIScrollPanel::GetVerticalScrollBar() const
{ return p_verticalScrollBar; }
UIScrollBar *UIScrollPanel::GetHorizontalScrollBar() const
{ return p_horizontalScrollBar; }

void UIScrollPanel::HandleScrollAreaRectTransform()
{
    RectTransform *scrollAreaRT =
            GetScrollArea()->GetGameObject()->GetComponent<RectTransform>();

    // Set margins to make room for the vertical scroll bar
    UIScrollBar *vScrollBar = GetVerticalScrollBar();
    GameObject *vScrollBarGo = vScrollBar->GetGameObject();
    int vScrollBarThickness = (vScrollBarGo->IsEnabled() ?
                                              vScrollBar->GetThickness() : 0 );
    if (GetVerticalScrollBarSide() == HorizontalSide::Right)
    {
        scrollAreaRT->SetMarginLeft(0);
        scrollAreaRT->SetMarginRight(vScrollBarThickness);
    }
    else
    {
        scrollAreaRT->SetMarginLeft(vScrollBarThickness);
        scrollAreaRT->SetMarginRight(0);
    }

    // Set margins to make room for the horizontal scroll bar
    UIScrollBar *hScrollBar = GetHorizontalScrollBar();
    GameObject *hScrollBarGo = hScrollBar->GetGameObject();
    RectTransform *vScrollBarRT = vScrollBarGo->GetComponent<RectTransform>();
    int hScrollBarThickness = (hScrollBarGo->IsEnabled() ?
                                          hScrollBar->GetThickness() : 0 );
    if (GetHorizontalScrollBarSide() == VerticalSide::Bot)
    {
        scrollAreaRT->SetMarginTop(0);
        vScrollBarRT->SetMarginTop(0);
        scrollAreaRT->SetMarginBot(hScrollBarThickness);
        vScrollBarRT->SetMarginBot(hScrollBarThickness);
    }
    else
    {
        scrollAreaRT->SetMarginTop(hScrollBarThickness);
        vScrollBarRT->SetMarginTop(hScrollBarThickness);
        scrollAreaRT->SetMarginBot(0);
        vScrollBarRT->SetMarginBot(0);
    }

    // Handle contained gameObject RectTransform anchors, depending on enabled
    // scrollings
    GameObject *containedGo = GetScrollArea()->GetContainedGameObject();
    if (containedGo)
    {
        RectTransform *containedGoRT = containedGo->GetComponent<RectTransform>();
        if (IsVerticalScrollEnabled() && IsHorizontalScrollEnabled())
        {
            containedGoRT->SetAnchors( Vector2(-1,1) );
        }
        else if (IsVerticalScrollEnabled() && !IsHorizontalScrollEnabled())
        {
            containedGoRT->SetAnchorX( Vector2(-1,1) );
            containedGoRT->SetAnchorY( Vector2(1) );
        }
        else if (!IsVerticalScrollEnabled() && IsHorizontalScrollEnabled())
        {
            containedGoRT->SetAnchorX( Vector2(-1) );
            containedGoRT->SetAnchorY( Vector2(-1,1) );
        }
        else
        {
            containedGoRT->SetAnchors( Vector2(-1,-1), Vector2(1,1) );
        }
    }
}

void UIScrollPanel::HandleScrollShowMode(const Vector2& contentSize,
                                         const Vector2& containerSize)
{
    bool showHorizontal = false;
    switch (GetHorizontalShowScrollMode())
    {
        case ShowScrollMode::Never: showHorizontal = false; break;
        case ShowScrollMode::WhenNeeded:
            showHorizontal = (contentSize.x > containerSize.x); break;
        case ShowScrollMode::Always: showHorizontal = true; break;
    }
    showHorizontal = showHorizontal && IsHorizontalScrollEnabled();

    bool showVertical = false;
    switch (GetVerticalShowScrollMode())
    {
        case ShowScrollMode::Never: showVertical = false; break;
        case ShowScrollMode::WhenNeeded:
            showVertical = (contentSize.y > containerSize.y); break;
        case ShowScrollMode::Always: showVertical = true; break;
    }
    showVertical = showVertical && IsVerticalScrollEnabled();

    GetHorizontalScrollBar()->GetGameObject()->SetEnabled(showHorizontal);
    GetVerticalScrollBar()->GetGameObject()->SetEnabled(showVertical);
}


UIScrollPanel *UIScrollPanel::CreateInto(GameObject *go)
{
    GameObjectFactory::CreateUIGameObjectInto(go);

    UIScrollPanel *scrollPanel = go->AddComponent<UIScrollPanel>();

    UIScrollArea *scrollArea = GameObjectFactory::CreateUIScrollArea();
    UIScrollBar *verticalScrollBar = GameObjectFactory::CreateUIScrollBar();
    UIScrollBar *horizontalScrollBar = GameObjectFactory::CreateUIScrollBar();

    go->SetAsChild(scrollArea->GetGameObject());
    go->SetAsChild(verticalScrollBar->GetGameObject());
    go->SetAsChild(horizontalScrollBar->GetGameObject());

    scrollPanel->p_scrollArea = scrollArea;
    scrollPanel->p_verticalScrollBar = verticalScrollBar;
    scrollPanel->p_horizontalScrollBar = horizontalScrollBar;

    scrollPanel->SetVerticalShowScrollMode(ShowScrollMode::Always);
    scrollPanel->SetHorizontalShowScrollMode(ShowScrollMode::Always);
    scrollPanel->SetVerticalScrollBarSide(HorizontalSide::Left);
    scrollPanel->SetHorizontalScrollBarSide(VerticalSide::Bot);
    scrollPanel->UpdateScrollUI(); // To avoid first frame being wrong

    return scrollPanel;
}

