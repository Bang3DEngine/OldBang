#include "Bang/UIScrollBar.h"

#include "Bang/Rect.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIFocusTaker.h"
#include "Bang/RectTransform.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/UITintedButton.h"
#include "Bang/UILayoutElement.h"
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
        Vector2i mouseCoords = Input::GetMouseCoords();
        Recti scrollRectPx = GetScrollingRect();
        Recti barRectPx = GetBar()->GetComponent<RectTransform>()->
                          GetScreenSpaceRectPx();
        if (!m_wasGrabbed)
        {
            m_grabOffset = (mouseCoords - barRectPx.GetMin());
        }

        Vector2i offsettedMouseCoords = mouseCoords - m_grabOffset;
        Vector2 leftSpace( scrollRectPx.GetSize() - barRectPx.GetSize() );
        Vector2 mousePercent = ((leftSpace != Vector2::Zero) ?
            Vector2(offsettedMouseCoords - scrollRectPx.GetMin()) / leftSpace :
            Vector2::Zero);
        float scrollPercent = mousePercent.GetAxis( GetAxis() );
        scrollPercent = Math::Clamp(scrollPercent, 0.0f, 1.0f);

        SetScrollingPercent(scrollPercent);
    }
    m_wasGrabbed = isBeingGrabbed;
}

void UIScrollBar::SetScrolling(int scrollingPx)
{
    m_scrollingPx = scrollingPx;

    Vector2i scrolling = (GetAxis() == Axis::Vertical) ?
                          Vector2i(0, scrollingPx) : Vector2i(scrollingPx, 0);
    GetScrollArea()->SetScrolling(scrolling);
}

void UIScrollBar::SetScrollingPercent(float percent)
{
    int scrollingPx = Math::Round<int>(percent * GetScrollingSpacePx());
    SetScrolling(-scrollingPx);
}

void UIScrollBar::SetLength(int lengthPx)
{
    UILayoutElement *barLE = GetBar()->GetComponent<UILayoutElement>();
    if (GetAxis() == Axis::Vertical) { barLE->SetPreferredHeight(lengthPx); }
    else { barLE->SetPreferredWidth(lengthPx); }

    barLE->IInvalidatable<ILayoutElement>::Invalidate();

    SetScrollingPercent( GetScrollingPercent() );
}

void UIScrollBar::SetLengthPercent(float lengthPercent)
{
    Vector2i length(
        Vector2::Round(Vector2(GetScrollingRect().GetSize()) * lengthPercent ) );
    SetLength(GetAxis() == Axis::Vertical ? length.y : length.x);
}

void UIScrollBar::SetThickness(int thickPx)
{
    m_thickness = thickPx;

    UILayoutElement *barLE = GetBar()->GetComponent<UILayoutElement>();
    if (GetAxis() == Axis::Vertical) { barLE->SetPreferredWidth(thickPx); }
    else { barLE->SetPreferredHeight(thickPx); }
}

void UIScrollBar::SetAxis(Axis axis)
{
    UILayoutElement *le = gameObject->GetComponent<UILayoutElement>();
    le->SetFlexibleHeight(GetAxis() == Axis::Vertical ? 1 : 0);
    le->SetFlexibleWidth (GetAxis() == Axis::Vertical ? 0 : 1);
    m_axis = axis;
}

int UIScrollBar::GetScrolling() const { return m_scrollingPx; }
float UIScrollBar::GetScrollingPercent() const
{
    return SCAST<float>(GetScrolling()) / GetScrollingSpacePx();
}

int UIScrollBar::GetLength() const { return m_length; }
int UIScrollBar::GetThickness() const { return m_thickness; }
Axis UIScrollBar::GetAxis() const { return m_axis; }

UIScrollBar *UIScrollBar::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);
    REQUIRE_COMPONENT(go, UIFocusTaker);

    UIScrollBar *scrollBar = go->AddComponent<UIScrollBar>();
    UILayoutElement *le = go->AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2::Zero );

    UIImageRenderer *bg = go->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::Yellow);

    UIScrollArea *scrollArea = GameObjectFactory::CreateUIScrollAreaInto(go);
    GameObject *container = scrollArea->GetContainer();
    UIVerticalLayout *contVL = container->AddComponent<UIVerticalLayout>();
    contVL->SetChildrenVerticalStretch(Stretch::None);
    contVL->SetChildrenHorizontalStretch(Stretch::None);
    contVL->SetChildrenHorizontalAlignment(HorizontalAlignment::Right);
    contVL->SetChildrenVerticalAlignment(VerticalAlignment::Top);
    contVL->SetPaddings(0);

    GameObject *bar = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *barLE = bar->AddComponent<UILayoutElement>();
    barLE->SetFlexibleSize( Vector2::Zero );

    UIImageRenderer *barImg = bar->AddComponent<UIImageRenderer>();
    barImg->SetTint(Color::Black);

    UITintedButton *btn = bar->AddComponent<UITintedButton>();
    btn->AddAgent(bar);
    btn->AddToTint(bar);
    btn->SetMode(UIButtonMode::UseRectTransform);
    btn->SetIdleTintColor( Color::Black );
    btn->SetOverTintColor( Color::Gray * 1.3f );
    btn->SetPressedTintColor( Color::Gray);

    scrollBar->p_bar = bar;
    scrollBar->p_button = btn;
    scrollBar->p_scrollArea = scrollArea;
    scrollBar->SetAxis(Axis::Vertical);
    scrollBar->SetLength(50);
    scrollBar->SetThickness(10);

    container->AddChild(bar);

    return scrollBar;
}

int UIScrollBar::GetScrollingSpacePx() const
{
    return GetScrollingRect().GetSize().GetAxis( GetAxis() );
}

Recti UIScrollBar::GetScrollingRect() const
{
    GameObject *cont = GetScrollArea()->gameObject;
    RectTransform *rt = cont->GetComponent<RectTransform>();
    return rt->GetScreenSpaceRectPx();
}

UIScrollArea *UIScrollBar::GetScrollArea() const { return p_scrollArea; }
GameObject *UIScrollBar::GetBar() const { return p_bar; }
UIButton *UIScrollBar::GetButton() const { return p_button; }

