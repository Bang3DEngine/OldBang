#include "Bang/UIScrollBar.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/AARect.h"
#include "Bang/GameObject.h"
#include "Bang/IFocusable.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

UIScrollBar::UIScrollBar()
{
}

UIScrollBar::~UIScrollBar()
{
}

void UIScrollBar::OnStart()
{
    Component::OnStart();
    GetFocusable()->EventEmitter<IFocusListener>::RegisterListener(this);
}

void UIScrollBar::OnUpdate()
{
    Component::OnUpdate();

    if ( IsBeingGrabbed() )
    {
        p_barImg->SetTint(Color::Black);

        Vector2 mouseCoordsPx (Input::GetMousePosition());
        AARect scrollRectPx = GetScrollingRect();
        AARect barRectPx( GetBar()->GetRectTransform()->GetViewportRect() );
        if (!m_wasGrabbed)
        {
            m_grabOffsetPx.x = mouseCoordsPx.x - barRectPx.GetMin().x;
            m_grabOffsetPx.y = (barRectPx.GetMax().y - mouseCoordsPx.y);
        }

        Vector2 mouseCoordsPxRel = mouseCoordsPx - scrollRectPx.GetMin();
        mouseCoordsPxRel.y = (scrollRectPx.GetMax().y - mouseCoordsPxRel.y);
        Vector2 offsettedMouseCoordsPxRel = mouseCoordsPxRel - m_grabOffsetPx;
        Vector2 emptySpacePx = scrollRectPx.GetSize() - barRectPx.GetSize();
        emptySpacePx = Vector2::Max(emptySpacePx, Vector2::One);

        Vector2 mousePercent = ((emptySpacePx != Vector2::Zero) ?
            Vector2(offsettedMouseCoordsPxRel - scrollRectPx.GetMin()) / emptySpacePx :
            Vector2::Zero);
        float scrollPercent = mousePercent.GetAxis( GetScrollAxis() );
        scrollPercent = Math::Clamp(scrollPercent, 0.0f, 1.0f);

        SetScrollingPercent(scrollPercent);
    }
    m_wasGrabbed = IsBeingGrabbed();

    if (Input::GetMouseButtonUp(MouseButton::Left))
    {
        if ( GetFocusable()->IsMouseOver() ) { OnMouseEnter(GetFocusable()); }
        else { OnMouseExit(GetFocusable()); }
    }
}

void UIScrollBar::SetSide(Side side)
{
    if (side != GetSide())
    {
        m_side = side;
        UpdateLengthThicknessMargins();
    }
}

void UIScrollBar::SetScrolling(int _scrollingPx)
{
    int scrollingPx = Math::Clamp(_scrollingPx, 0, GetScrollingSpacePx());
    m_scrollingPx = scrollingPx;

    Vector2i scrolling = (GetScrollAxis() == Axis::Vertical) ?
                          Vector2i(0, -scrollingPx) : Vector2i(scrollingPx, 0);
    GetScrollArea()->SetScrolling(scrolling);
}

void UIScrollBar::SetScrollingPercent(float _percent)
{
    float percent = Math::Clamp(_percent, 0.0f, 1.0f);
    int scrollingPx = Math::Round<int>(percent * GetScrollingSpacePx());
    SetScrolling(scrollingPx);
}

void UIScrollBar::SetLength(int lengthPx)
{
    if (lengthPx != GetLength())
    {
        m_length = lengthPx;
        UpdateLengthThicknessMargins();
    }
}

void UIScrollBar::SetLengthPercent(float lengthPercent)
{
    Vector2i length(
        Vector2::Round(Vector2(GetScrollingRect().GetSize()) * lengthPercent ) );
    SetLength(GetScrollAxis() == Axis::Vertical ? length.y : length.x);
}

void UIScrollBar::SetThickness(int thickPx)
{
    if (thickPx != GetThickness())
    {
        m_thickness = thickPx;
        UpdateLengthThicknessMargins();
    }
}

Side UIScrollBar::GetSide() const { return m_side; }
int UIScrollBar::GetScrolling() const { return m_scrollingPx; }
float UIScrollBar::GetScrollingPercent() const
{
    int scrollSpacePx = GetScrollingSpacePx();
    return scrollSpacePx > 0 ? Cast<float>(GetScrolling()) / scrollSpacePx : 0;
}

int UIScrollBar::GetLength() const { return m_length; }
int UIScrollBar::GetThickness() const { return m_thickness; }
Axis UIScrollBar::GetScrollAxis() const
{
    switch (GetSide())
    {
        case Side::Left: case Side::Right: return Axis::Vertical;
        case Side::Top: case Side::Bot: return Axis::Horizontal;
    }
    ASSERT(false); return Axis::Horizontal;
}

bool UIScrollBar::IsBeingGrabbed() const
{
    return GetFocusable()->IsBeingPressed();
}

void UIScrollBar::UpdateLengthThicknessMargins()
{
    RectTransform *rt = GetGameObject()->GetRectTransform();
    RectTransform *barRT = GetBar()->GetRectTransform();
    if (GetScrollAxis() == Axis::Horizontal)
    {
        bool bot = (GetSide() == Side::Bot);
        rt->SetAnchorX( Vector2(-1, 1) );
        rt->SetAnchorY( Vector2(bot ? -1 : 1 ) );
        rt->SetMargins(0);
        rt->SetMarginTop(bot ? -GetThickness() : 0);
        rt->SetMarginBot(bot ? 0 : -GetThickness());

        barRT->SetAnchorX( Vector2(-1) );
        barRT->SetAnchorY( Vector2(-1, 1) );
        barRT->SetMargins(0, 0, -GetLength(), 0);
    }
    else
    {
        bool left = (GetSide() == Side::Left);
        rt->SetAnchorX( Vector2( left ? -1 : 1 )  );
        rt->SetAnchorY( Vector2(-1, 1) );
        rt->SetMargins(0);
        rt->SetMarginRight(left ? -GetThickness() : 0);
        rt->SetMarginLeft(left ? 0 : -GetThickness());

        barRT->SetAnchorX( Vector2(-1, 1) );
        barRT->SetAnchorY( Vector2(1) );
        barRT->SetMargins(0, 0, 0, -GetLength());
    }
    SetScrolling( GetScrolling() );
}

UIScrollBar *UIScrollBar::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    go->SetName("ScrollBar");
    UIScrollBar *scrollBar = go->AddComponent<UIScrollBar>();
    go->AddComponent<UIFocusable>();

    UIScrollArea *scrollArea = GameObjectFactory::CreateUIScrollAreaInto(go);
    GameObject *bar = GameObjectFactory::CreateUIGameObjectNamed("Bar");

    UIImageRenderer *barImg = bar->AddComponent<UIImageRenderer>();
    barImg->SetTint(Color::Black);

    UIFocusable *btn = bar->AddComponent<UIFocusable>();

    scrollBar->p_bar = bar;
    scrollBar->p_button = btn;
    scrollBar->p_barImg = barImg;
    scrollBar->p_scrollArea = scrollArea;
    scrollBar->SetSide(Side::Left);
    scrollBar->SetLength(50);
    scrollBar->SetThickness(10);

    scrollArea->SetContainedGameObject(bar);

    return scrollBar;
}

int UIScrollBar::GetScrollingSpacePx() const
{
    if (!GetGameObject()->IsEnabled() || !IsEnabled()) { return 0; }
    int scrollingSpace = GetScrollingRect().GetSize().GetAxis( GetScrollAxis() );
    scrollingSpace -= GetLength();
    return  Math::Max(scrollingSpace, 0);
}

AARect UIScrollBar::GetScrollingRect() const
{
    GameObject *cont = GetScrollArea()->GetGameObject();
    RectTransform *rt = cont->GetRectTransform();
    return AARect( rt->GetViewportRect() );
}

UIScrollArea *UIScrollBar::GetScrollArea() const { return p_scrollArea; }
GameObject *UIScrollBar::GetBar() const { return p_bar; }

void UIScrollBar::OnMouseEnter(IFocusable*)
{
    if (!IsBeingGrabbed())
    {
        p_barImg->SetTint(Color::DarkGray);
    }
}

void UIScrollBar::OnMouseExit(IFocusable*)
{
    if (!IsBeingGrabbed()) { p_barImg->SetTint(Color::Black); }
}

UIFocusable *UIScrollBar::GetFocusable() const { return p_button; }

