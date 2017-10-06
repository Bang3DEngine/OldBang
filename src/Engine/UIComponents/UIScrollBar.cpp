#include "Bang/UIScrollBar.h"

#include "Bang/UIScrollArea.h"
#include "Bang/UIFocusTaker.h"
#include "Bang/RectTransform.h"
#include "Bang/UIFrameLayout.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"

USING_NAMESPACE_BANG

UIScrollBar::UIScrollBar()
{
}

UIScrollBar::~UIScrollBar()
{
}

void UIScrollBar::ScrollTo(float percent)
{
    int scrollingPx = (percent * 100);
    Vector2i scrolling = IsVertical() ? Vector2i(scrollingPx, 0) :
                                        Vector2i(0, scrollingPx);
    GetScrollArea()->SetScrolling(scrolling);
}

void UIScrollBar::SetLength(int lengthPx)
{
    UILayoutElement *barLE = GetBar()->GetComponent<UILayoutElement>();
    if (IsVertical())
    { barLE->SetMinHeight(lengthPx); barLE->SetPreferredHeight(lengthPx); }
    else
    { barLE->SetMinWidth (lengthPx); barLE->SetPreferredWidth(lengthPx); }
}

void UIScrollBar::SetThickness(int thickPx)
{
    UILayoutElement *barLE = GetBar()->GetComponent<UILayoutElement>();
    if (IsVertical())
    { barLE->SetMinWidth(thickPx);  barLE->SetPreferredWidth(thickPx); }
    else
    { barLE->SetMinHeight(thickPx); barLE->SetPreferredHeight(thickPx); }
}

void UIScrollBar::SetVertical(bool vertical)
{
    UILayoutElement *le = gameObject->GetComponent<UILayoutElement>();
    le->SetFlexibleHeight(vertical ? 1 : 0);
    le->SetFlexibleWidth (vertical ? 0 : 1);
}

UIScrollArea *UIScrollBar::GetScrollArea()
{
    return p_scrollArea;
}

bool UIScrollBar::IsVertical() const
{
    UILayoutElement *le = gameObject->GetComponent<UILayoutElement>();
    return le->GetFlexibleHeight() == 1;
}

GameObject *UIScrollBar::GetBar() const { return p_bar; }

UIScrollBar *UIScrollBar::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);
    REQUIRE_COMPONENT(go, UIFocusTaker);

    UIScrollBar *scrollBar = go->AddComponent<UIScrollBar>();
    UILayoutElement *le = go->AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2::Zero );

    UIImageRenderer *bg = go->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::LightGray);

    UIScrollArea *scrollArea = GameObjectFactory::CreateUIScrollAreaInto(go);
    GameObject *container = scrollArea->GetContainer();
    UIFrameLayout *contFL = container->AddComponent<UIFrameLayout>();
    contFL->SetChildrenVerticalStretch(Stretch::None);
    contFL->SetChildrenHorizontalStretch(Stretch::None);
    contFL->SetPaddings(4);

    GameObject *bar = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *barLE = bar->AddComponent<UILayoutElement>();
    barLE->SetFlexibleSize( Vector2::Zero );

    UIImageRenderer *barImg = bar->AddComponent<UIImageRenderer>();
    barImg->SetTint(Color::Black);

    scrollBar->p_bar = bar;
    scrollBar->p_scrollArea = scrollArea;
    scrollBar->SetVertical(true);
    scrollBar->SetLength(50);
    scrollBar->SetThickness(10);

    container->AddChild(bar);

    return scrollBar;
}

