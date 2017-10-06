#include "Bang/UIScrollPanel.h"

#include "Bang/GameObject.h"
#include "Bang/UIScrollBar.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIFocusTaker.h"
#include "Bang/RectTransform.h"
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
    GetScrollBar()->SetLength(25);
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

