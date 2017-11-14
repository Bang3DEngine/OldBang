#include "Bang/UIList.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/GameObject.h"
#include "Bang/UIScrollBar.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

USING_NAMESPACE_BANG

UIList::UIList()
{
}

UIList::~UIList()
{
}

void UIList::OnUpdate()
{
    if (GetGameObject()->GetComponent<RectTransform>()->IsMouseOver())
    {
        // Selection In/Out
        if (Input::GetKeyDownRepeat(Key::Down))
        {
            SetSelection( GetSelectedIndex() + 1 );
        }

        if (Input::GetKeyDownRepeat(Key::Up))
        {
            SetSelection( GetSelectedIndex() - 1 );
        }

        // Mouse In/Out
        GOItem *itemUnderMouse = nullptr;
        for (GOItem *childItem : GetContainer()->GetChildren())
        {
            RectTransform *rt = childItem->GetComponent<RectTransform>();
            if (rt && rt->IsMouseOver()) { itemUnderMouse = childItem; break; }
        }

        if (p_itemUnderMouse != itemUnderMouse)
        {
            if (p_itemUnderMouse) { Callback(p_itemUnderMouse, Action::MouseOut); }

            p_itemUnderMouse = itemUnderMouse;
            if (p_itemUnderMouse) { Callback(p_itemUnderMouse, Action::MouseOver); }
        }

        // Clicked
        if (Input::GetKeyDownRepeat(Key::Right) ||
            Input::GetKeyDownRepeat(Key::Enter))
        {
            GOItem *selectedItem = GetSelectedItem();
            if (selectedItem) { Callback(selectedItem, Action::Pressed); }
        }

        if (p_itemUnderMouse)
        {
            if (Input::GetMouseButtonDown(MouseButton::Left))
            {
                SetSelection(p_itemUnderMouse);
                Callback(p_itemUnderMouse, Action::ClickedLeft);
            }

            if (Input::GetMouseButtonDown(MouseButton::Right))
            {
                Callback(p_itemUnderMouse, Action::ClickedRight);
            }

            if (Input::GetMouseButtonDoubleClick(MouseButton::Left))
            {
                Callback(p_itemUnderMouse, Action::DoubleClickedLeft);
            }
        }
    }
    else
    {
        if (p_itemUnderMouse) { Callback(p_itemUnderMouse, Action::MouseOut); }
        p_itemUnderMouse = nullptr;
    }
}

void UIList::AddItem(GOItem *newItem)
{
    bool hadSelectedGameObject = GetSelectedItem();

    newItem->SetParent(GetContainer());

    if (!hadSelectedGameObject) { SetSelection(0); }
}

void UIList::RemoveItem(GOItem *item)
{
    GameObject::Destroy(item);

    int selIndex = GetSelectedIndex();
    if (p_itemUnderMouse == item) { p_itemUnderMouse = nullptr; }
    selIndex = Math::Clamp(selIndex, 0, GetContainer()->GetChildren().Size());
    SetSelection(selIndex);
}

void UIList::Clear()
{
    List<GOItem*> childrenItems = GetContainer()->GetChildren();
    for (GOItem *child : childrenItems) { RemoveItem(child); }
    GetScrollPanel()->SetScrollingPercent( Vector2(0.0f) );
}

void UIList::SetSelection(int _i)
{
    const int numChildren = GetContainer()->GetChildren().Size();
    ENSURE(numChildren > 0);

    int i = ((_i + numChildren) % numChildren);
    if (m_selectionIndex != i)
    {
        GOItem *prevSelectedItem = GetSelectedItem();
        if (prevSelectedItem) { Callback(prevSelectedItem, Action::SelectionOut); }

        m_selectionIndex = i;
        GOItem *selectedItem = GetSelectedItem();
        if (selectedItem) { Callback(selectedItem, Action::SelectionIn); }
    }
}

void UIList::SetSelection(GOItem *item)
{
    SetSelection(GetContainer()->GetChildren().IndexOf(item));
}

GameObject *UIList::GetContainer() const
{
    return p_container;
}

int UIList::GetSelectedIndex() const
{
    return m_selectionIndex;
}

GOItem *UIList::GetSelectedItem() const
{
    return GetContainer()->GetChild( GetSelectedIndex() );
}

void UIList::SetSelectionCallback(SelectionCallback selectionCallback)
{
    m_selectionCallback = selectionCallback;
}

UIList* UIList::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UIScrollPanel *scrollPanel = GameObjectFactory::CreateUIScrollPanelInto(go);

    UILayoutElement *le = go->AddComponent<UILayoutElement>();
    le->SetMinHeight(10);

    GameObject *container = GameObjectFactory::CreateUIGameObject();

    UIDirLayout *dirLayout;
    const bool vertical = true;
    if (vertical) { dirLayout = container->AddComponent<UIVerticalLayout>(); }
    else { dirLayout = container->AddComponent<UIHorizontalLayout>(); }
    dirLayout->SetSpacing(0);
    dirLayout->SetPaddings(0);

    container->GetComponent<RectTransform>()->SetPivotPosition(Vector2(-1,1));
    UIContentSizeFitter *csf = container->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);

    scrollPanel->GetScrollArea()->SetContainedGameObject(container);

    UIList *ld = go->AddComponent<UIList>();
    ld->p_scrollPanel = scrollPanel;
    ld->p_container = container;

    return ld;
}

void UIList::Callback(GOItem *item, Action action)
{
    if (m_selectionCallback) { m_selectionCallback(item, action); }
}

UIScrollPanel *UIList::GetScrollPanel() const { return p_scrollPanel; }
