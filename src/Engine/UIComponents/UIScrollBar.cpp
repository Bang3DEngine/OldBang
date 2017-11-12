#include "Bang/UIScrollBar.h"

#include "Bang/Rect.h"
#include "Bang/UIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/UITintedButton.h"
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

    bool isBeingGrabbed = GetButton()->IsBeingPressed();
    if (isBeingGrabbed)
    {
        Vector2 mouseCoords (Input::GetMouseCoords());
        Rect scrollRectPx = GetScrollingRect();
        Rect barRectPx = GetBar()->GetComponent<RectTransform>()->
                         GetScreenSpaceRectPx();
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
    m_wasGrabbed = isBeingGrabbed;
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
    m_length = lengthPx;
    UpdateLengthThicknessMargins();
}

void UIScrollBar::SetLengthPercent(float lengthPercent)
{
    Vector2i length(
        Vector2::Round(Vector2(GetScrollingRect().GetSize()) * lengthPercent ) );
    SetLength(GetScrollAxis() == Axis::Vertical ? length.y : length.x);
}

void UIScrollBar::SetThickness(int thickPx)
{
    m_thickness = thickPx;
    UpdateLengthThicknessMargins();
}

Side UIScrollBar::GetSide() const { return m_side; }
int UIScrollBar::GetScrolling() const { return m_scrollingPx; }
float UIScrollBar::GetScrollingPercent() const
{
    int scrollSpacePx = GetScrollingSpacePx();
    return scrollSpacePx > 0 ? SCAST<float>(GetScrolling()) / scrollSpacePx : 0;
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

void UIScrollBar::UpdateLengthThicknessMargins()
{
    RectTransform *rt = GetGameObject()->GetComponent<RectTransform>();
    RectTransform *barRT = GetBar()->GetComponent<RectTransform>();
    if (GetScrollAxis() == Axis::Horizontal)
    {
        bool bot = (GetSide() == Side::Bot);
        rt->SetAnchorX( Vector2(-1, 1) );
        rt->SetAnchorY( Vector2(bot ? 1 : -1 ) );
        rt->SetMargins(0, bot ? -GetThickness() : 0,
                       0, bot ? 0 : -GetThickness());

        barRT->SetAnchorX( Vector2(-1) );
        barRT->SetAnchorY( Vector2(-1, 1) );
        barRT->SetMargins(0, 0, -GetLength(), 0);
    }
    else
    {
        bool left = (GetSide() == Side::Left);
        rt->SetAnchorX( Vector2( left ? -1 : 1 )  );
        rt->SetAnchorY( Vector2(-1, 1) );
        rt->SetMargins(left ? 0 : -GetThickness(), 0,
                       left ? -GetThickness() : 0, 0);

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
    GameObject *bar = GameObjectFactory::CreateUIGameObject("Bar");

    UIImageRenderer *barImg = bar->AddComponent<UIImageRenderer>();
    barImg->SetTint(Color::Black);

    UITintedButton *btn = bar->AddComponent<UITintedButton>();
    btn->RegisterButtonPart(bar);
    btn->AddToTint(bar);
    btn->SetMode(UIButtonMode::UseRectTransform);
    btn->SetIdleTintColor( Color::Black );
    btn->SetOverTintColor( Color::Gray * 1.3f );
    btn->SetPressedTintColor( Color::Gray);

    scrollBar->p_bar = bar;
    scrollBar->p_button = btn;
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
    RectTransform *rt = cont->GetComponent<RectTransform>();
    return rt->GetScreenSpaceRectPx();
}

UIScrollArea *UIScrollBar::GetScrollArea() const { return p_scrollArea; }
GameObject *UIScrollBar::GetBar() const { return p_bar; }
UIButton *UIScrollBar::GetButton() const { return p_button; }

