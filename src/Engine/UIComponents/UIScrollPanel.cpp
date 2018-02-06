#include "Bang/UIScrollPanel.h"

#include "Bang/Input.h"
#include "Bang/Color.h"
#include "Bang/AARect.h"
#include "Bang/UICanvas.h"
#include "Bang/GameObject.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIScrollBar.h"
#include "Bang/UIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

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
        contentSize = Vector2::Max(contentSize, Vector2::One);

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
        if (GetGameObject()->GetRectTransform()->IsMouseOver(false))
        {
            Vector2i mouseWheelPx(Input::GetMouseWheel() * WheelScrollSpeedPx);

            Vector2 mouseWheelPercent = Vector2(mouseWheelPx) / contentSize;
            scrollingPercent -= mouseWheelPercent;
        }
        scrollingPercent = Vector2::Clamp(scrollingPercent, Vector2::Zero,
                                                            Vector2::One);

        // Apply scrollings
        Vector2i scrollEnabledMask(IsHorizontalScrollEnabledAndNoFit() ? 1 : 0,
                                   IsVerticalScrollEnabledAndNoFit()   ? 1 : 0);
        scrollingPercent *= Vector2(scrollEnabledMask);

        Vector2 scrollMaxAmount = (contentSize - containerSize);
        scrollMaxAmount = Vector2::Clamp(scrollMaxAmount, Vector2::Zero,
                                         contentSize);
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

    // Clamp to adjust for dynamic children size changes
    Vector2i clampedScrolling = Vector2i::Min(GetScrolling(), Vector2i(contentSize));
    SetScrolling(clampedScrolling);
}

void UIScrollPanel::OnUpdate()
{
    Component::OnUpdate();

    UpdateScrollUI();
}

void UIScrollPanel::OnPostUpdate()
{
    Component::OnPostUpdate();

    UIScrollArea *sa = GetScrollArea();
    if (sa->GetContainedGameObject())
    {
        RectTransform *referenceRT = sa->GetGameObject()->GetRectTransform();
        RectTransform *toConvertRT = sa->GetContainedGameObject()->GetRectTransform();
        RectTransform *toConvertParentRT = toConvertRT->GetGameObject()
                                           ->GetParent()->GetRectTransform();

        AARect refRect = referenceRT->GetViewportRect();

        Vector2 newAnchorMin = toConvertParentRT->
                               FromViewportPointToLocalPointNDC(refRect.GetMin());
        Vector2 newAnchorMax = toConvertParentRT->
                               FromViewportPointToLocalPointNDC(refRect.GetMax());

        constexpr float Epsilon = 0.0001f;
        if (GetForceHorizontalFit() &&
            (Math::Abs(newAnchorMin.x - toConvertRT->GetAnchorMin().x) > Epsilon ||
             Math::Abs(newAnchorMax.x - toConvertRT->GetAnchorMax().x) > Epsilon) )
        {
            toConvertRT->SetAnchorMinX( newAnchorMin.x );
            toConvertRT->SetAnchorMaxX( newAnchorMax.x );
        }

        if (GetForceVerticalFit() &&
            (Math::Abs(newAnchorMin.y - toConvertRT->GetAnchorMin().y) > Epsilon ||
             Math::Abs(newAnchorMax.y - toConvertRT->GetAnchorMax().y) > Epsilon) )
        {
            toConvertRT->SetAnchorMinY( newAnchorMin.y );
            toConvertRT->SetAnchorMaxY( newAnchorMax.y );
        }
    }
}

void UIScrollPanel::SetForceVerticalFit(bool forceVerticalFit)
{
    m_forceVerticalFit = forceVerticalFit;
}

void UIScrollPanel::SetForceHorizontalFit(bool forceHorizontalFit)
{
    m_forceHorizontalFit = forceHorizontalFit;
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
    Vector2 scrollPerc;
    const Vector2 maxScrollLength = GetMaxScrollLength();
    scrollPerc.x = (maxScrollLength.x > 0 ? scrolling.x / maxScrollLength.x : 0);
    scrollPerc.y = (maxScrollLength.y > 0 ? scrolling.y / maxScrollLength.y : 0);
    SetScrollingPercent(scrollPerc);
}

void UIScrollPanel::SetScrollingPercent(const Vector2 &scrollPerc)
{
    Vector2 scrollPercClamped = Vector2::Clamp(scrollPerc,
                                               Vector2::Zero, Vector2::One);
    GetScrollArea()->SetScrolling(
        Vector2i( Vector2::Round(scrollPercClamped * GetMaxScrollLength())) );
    GetHorizontalScrollBar()->SetScrollingPercent( scrollPercClamped.x );
    GetVerticalScrollBar()->SetScrollingPercent( scrollPercClamped.y );
}

Vector2i UIScrollPanel::GetScrolling() const
{ return GetScrollArea()->GetScrolling(); }
bool UIScrollPanel::GetForceVerticalFit() const
{ return m_forceVerticalFit; }
bool UIScrollPanel::GetForceHorizontalFit() const
{ return m_forceHorizontalFit; }
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
    return containedGo ? containedGo->GetRectTransform()->
                            GetViewportRect().GetSize() : Vector2::Zero;
}

Vector2 UIScrollPanel::GetContainerSize() const
{
    return GetGameObject()->GetRectTransform()->GetViewportRect().GetSize();
}

Vector2 UIScrollPanel::GetMaxScrollLength() const
{
    return Vector2::Max(GetContentSize() - GetContainerSize(), Vector2::Zero);
}

UIScrollArea *UIScrollPanel::GetScrollArea() const { return p_scrollArea; }
UIScrollBar *UIScrollPanel::GetVerticalScrollBar() const
{ return p_verticalScrollBar; }
UIScrollBar *UIScrollPanel::GetHorizontalScrollBar() const
{ return p_horizontalScrollBar; }

void UIScrollPanel::HandleScrollAreaRectTransform()
{
    RectTransform *scrollAreaRT =
            GetScrollArea()->GetGameObject()->GetRectTransform();

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
    RectTransform *vScrollBarRT = vScrollBarGo->GetRectTransform();
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
        RectTransform *containedGoRT = containedGo->GetRectTransform();
        if (IsVerticalScrollEnabledAndNoFit() &&
            IsHorizontalScrollEnabledAndNoFit())
        {
            containedGoRT->SetAnchors( Vector2(-1,1) );
        }
        else if ( IsVerticalScrollEnabledAndNoFit() &&
                 !IsHorizontalScrollEnabledAndNoFit())
        {
            containedGoRT->SetAnchorX( Vector2(-1,1) );
            containedGoRT->SetAnchorY( Vector2(1) );
        }
        else if (!IsVerticalScrollEnabledAndNoFit() &&
                  IsHorizontalScrollEnabledAndNoFit())
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
    showHorizontal = showHorizontal && IsHorizontalScrollEnabledAndNoFit();

    bool showVertical = false;
    switch (GetVerticalShowScrollMode())
    {
        case ShowScrollMode::Never: showVertical = false; break;
        case ShowScrollMode::WhenNeeded:
            showVertical = (contentSize.y > containerSize.y); break;
        case ShowScrollMode::Always: showVertical = true; break;
    }
    showVertical = showVertical && IsVerticalScrollEnabledAndNoFit();

    GetHorizontalScrollBar()->GetGameObject()->SetEnabled(showHorizontal);
    GetVerticalScrollBar()->GetGameObject()->SetEnabled(showVertical);
}

bool UIScrollPanel::IsVerticalScrollEnabledAndNoFit() const
{
    return IsVerticalScrollEnabled() && !GetForceVerticalFit();
}

bool UIScrollPanel::IsHorizontalScrollEnabledAndNoFit() const
{
    return IsHorizontalScrollEnabled() && !GetForceHorizontalFit();
}

UIScrollPanel *UIScrollPanel::CreateInto(GameObject *go)
{
    GameObjectFactory::CreateUIGameObjectInto(go);

    UIScrollPanel *scrollPanel = go->AddComponent<UIScrollPanel>();

    UIScrollArea *scrollArea = GameObjectFactory::CreateUIScrollArea();
    UIScrollBar *verticalScrollBar = GameObjectFactory::CreateUIScrollBar();
    UIScrollBar *horizontalScrollBar = GameObjectFactory::CreateUIScrollBar();

    scrollArea->GetGameObject()->SetParent(go);
    verticalScrollBar->GetGameObject()->SetParent(go);
    horizontalScrollBar->GetGameObject()->SetParent(go);

    scrollPanel->p_scrollArea = scrollArea;
    scrollPanel->p_verticalScrollBar = verticalScrollBar;
    scrollPanel->p_horizontalScrollBar = horizontalScrollBar;

    scrollPanel->p_scrollArea->GetGameObject()->
                 GetRectTransform()->TranslateLocal( Vector3(0, 0, 0.1f) );

    scrollPanel->SetVerticalShowScrollMode(ShowScrollMode::Always);
    scrollPanel->SetHorizontalShowScrollMode(ShowScrollMode::Always);
    scrollPanel->SetVerticalScrollBarSide(HorizontalSide::Left);
    scrollPanel->SetHorizontalScrollBarSide(VerticalSide::Bot);
    scrollPanel->UpdateScrollUI(); // To avoid first frame being wrong

    return scrollPanel;
}

