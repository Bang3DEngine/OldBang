#include "Bang/UIScrollBar.h"

#include "Bang/Rect.h"
#include "Bang/GameObject.h"
#include "Bang/UIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/UIButtoneable.h"
#include "Bang/UIImageRenderer.h"

USING_NAMESPACE_BANG

UIScrollBar::UIScrollBar()
{
}

UIScrollBar::~UIScrollBar()
{
}

void UIScrollBar::OnUpdate()
{
    Component::OnUpdate();

    if ( IsBeingGrabbed() )
    {
        p_barImg->SetTint(Color::Black);

        Vector2 mouseCoords (Input::GetMouseCoords());
        Rect scrollRectPx = GetScrollingRect();
        Rect barRectPx = GetBar()->GetRectTransform()->GetScreenSpaceRectPx();
        if (!m_wasGrabbed)
        {
            m_grabOffset = Vector2i( Vector2::Round(mouseCoords -
                                                    barRectPx.GetMin()) );
        }

        Vector2 offsettedMouseCoords = mouseCoords - Vector2(m_grabOffset);
        Vector2 leftSpace = scrollRectPx.GetSize() - barRectPx.GetSize();
        Vector2 mousePercent = ((leftSpace != Vector2::Zero) ?
            Vector2(offsettedMouseCoords - scrollRectPx.GetMin()) / leftSpace :
            Vector2::Zero);
        float scrollPercent = mousePercent.GetAxis( GetScrollAxis() );
        scrollPercent = Math::Clamp(scrollPercent, 0.0f, 1.0f);

        SetScrollingPercent(scrollPercent);
    }
    m_wasGrabbed = IsBeingGrabbed();
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
    return GetButton()->IsBeingPressed();
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

    UIScrollArea *scrollArea = GameObjectFactory::CreateUIScrollAreaInto(go);
    GameObject *bar = GameObjectFactory::CreateUIGameObjectNamed("Bar");

    UIImageRenderer *barImg = bar->AddComponent<UIImageRenderer>();
    barImg->SetTint(Color::Black);

    UIButtoneable *btn = bar->AddComponent<UIButtoneable>();
    btn->RegisterButtonPart(bar);
    btn->SetMode(UIButtoneableMode::RectTransform);

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
    int scrollingSpace = GetScrollingRect().GetSize().GetAxis( GetScrollAxis() );
    scrollingSpace -= GetLength();
    return  Math::Max(scrollingSpace, 0);
}

Rect UIScrollBar::GetScrollingRect() const
{
    GameObject *cont = GetScrollArea()->GetGameObject();
    RectTransform *rt = cont->GetRectTransform();
    return rt->GetScreenSpaceRectPx();
}

UIScrollArea *UIScrollBar::GetScrollArea() const { return p_scrollArea; }
GameObject *UIScrollBar::GetBar() const { return p_bar; }

void UIScrollBar::OnButton_MouseEnter(UIButtoneable*)
{
    if (!IsBeingGrabbed())
    {
        p_barImg->SetTint(Color::LightGray);
    }
}

void UIScrollBar::OnButton_MouseExit(UIButtoneable*)
{
    if (!IsBeingGrabbed())
    {
        p_barImg->SetTint(Color::Gray);
    }
}

void UIScrollBar::OnButton_MouseDown(UIButtoneable *btn, MouseButton mb)
{
    Debug_Log("DOWN");
}

void UIScrollBar::OnButton_MouseUp(UIButtoneable *btn, MouseButton mb, bool inside)
{
    if (inside) { OnButton_MouseEnter(btn); }
    else { OnButton_MouseExit(btn); }
}

UIButtoneable *UIScrollBar::GetButton() const { return p_button; }

