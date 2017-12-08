#include "Bang/UIList.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/UICanvas.h"
#include "Bang/IFocusable.h"
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
    Component::OnUpdate();

    if ( UICanvas::IsMouseOver(GetGameObject(), true) )
    {
        // Mouse In/Out
        GOItem *itemUnderMouse = nullptr;
        if (UICanvas::IsMouseOver(this, true))
        {
            for (GOItem *childItem : p_items)
            {
                if (UICanvas::IsMouseOver(childItem, true))
                {
                    itemUnderMouse = childItem;
                    break;
                }
            }
        }

        if (p_itemUnderMouse != itemUnderMouse)
        {
            if (p_itemUnderMouse)
            {
                Callback(p_itemUnderMouse, Action::MouseOut);
            }

            p_itemUnderMouse = itemUnderMouse;
            if (p_itemUnderMouse)
            {
                Callback(p_itemUnderMouse, Action::MouseOver);
            }
        }
    }

    if (SomeChildHasFocus())
    {
        HandleShortcuts();

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
    else if (!p_itemUnderMouse)
    {
        SetSelection(nullptr);
        if (p_itemUnderMouse) { Callback(p_itemUnderMouse, Action::MouseOut); }
        p_itemUnderMouse = nullptr;
    }
}

void UIList::AddItem(GOItem *newItem)
{
    bool hadSelectedGameObject = GetSelectedItem();

    p_items.PushBack(newItem);

    List<IFocusable*> newItemFocusables =
                            newItem->GetComponentsInChildren<IFocusable>(true);
    for (IFocusable* newItemFocusable : newItemFocusables)
    {
        newItemFocusable->EventEmitter<IFocusListener>::RegisterListener(this);
    }

    newItem->EventEmitter<IDestroyListener>::RegisterListener(this);
    newItem->SetParent(GetContainer());

    if (!hadSelectedGameObject) { SetSelection(0); }
}

void UIList::RemoveItem(GOItem *item)
{
    ASSERT( p_items.Contains(item) );

    int indexOfItem = p_items.IndexOf(item);

    // Change selection to another element
    if (p_itemUnderMouse == item) { p_itemUnderMouse = nullptr; }
    if (GetNumItems() > 0)
    {
        int newSelIndex = GetSelectedIndex() == 0 ? 1 : (GetSelectedIndex()-1);
        newSelIndex = Math::Clamp(newSelIndex, 0, GetNumItems()-1-1);
        SetSelection(-1); // Deselect first
        SetSelection(newSelIndex);
    }
    else { ClearSelection(); }

    // Destroy the element
    GameObject::Destroy(item);
    if (indexOfItem < GetSelectedIndex()) { m_selectionIndex -= 1; }
    p_items.Remove(item);
}

void UIList::ClearSelection()
{
    if (GetSelectedIndex() >= 0)
    {
        if (m_selectionCallback)
        {
            GOItem *selectedItem = GetSelectedItem();
            if (selectedItem)
            {
                m_selectionCallback(selectedItem, Action::SelectionOut);
            }
        }
        SetSelection(-1);
    }
}

void UIList::Clear()
{
    while (!p_items.IsEmpty()) { RemoveItem(p_items.Front()); }
    GetScrollPanel()->SetScrollingPercent( Vector2(0.0f) );
    ClearSelection();
}

const Array<GOItem *> &UIList::GetItems() const { return p_items; }
GOItem *UIList::GetItem(int i) const
{
    if (i >= 0 && i < p_items.Size())
    {
        // auto it = GetItems().Begin();
        // std::advance(it, GetSelectedIndex());
        // return *it;
        return GetItems()[i];
    }
    return nullptr;
}

void UIList::ScrollTo(int i)
{
    ScrollTo( GetItem(i) );
}

void UIList::ScrollTo(GOItem *item)
{
    Rect itemRect = item->GetRectTransform()-> GetScreenSpaceRectPx();
    Rect panelRect = GetScrollPanel()->GetGameObject()->GetRectTransform()->
                                                        GetScreenSpaceRectPx();
    Rect containerRect = GetContainer()->GetRectTransform()->
                                         GetScreenSpaceRectPx();
    Rect relativeItemRect = itemRect - containerRect.GetMin();

    Vector2i scrolling = -Vector2i::One;
    if (itemRect.GetMax().y > panelRect.GetMax().y)
    {
        scrolling = Vector2i(relativeItemRect.GetMax() - panelRect.GetHeight());
    }
    else if (itemRect.GetMin().y < panelRect.GetMin().y)
    {
        scrolling = Vector2i(relativeItemRect.GetMin());
    }

    if (scrolling != -Vector2i::One)
    {
        GetScrollPanel()->SetScrolling(scrolling);
    }
}

int UIList::GetNumItems() const
{
    return p_items.Size();
}

void UIList::SetSelection(int index)
{
    if (GetSelectedIndex() != index)
    {
        GOItem *prevSelectedItem = GetSelectedItem();
        if (prevSelectedItem) { Callback(prevSelectedItem, Action::SelectionOut); }
    }

    if (GetSelectedIndex() != index && index >= 0 && index < GetNumItems())
    {
        m_selectionIndex = index;
        GOItem *selectedItem = GetSelectedItem();
        if (selectedItem)
        {
            ScrollTo(selectedItem);

            IFocusable *itemFocusable = selectedItem->
                                        GetComponentInChildren<IFocusable>();
            UICanvas::GetActive()->SetFocus(itemFocusable);

            Callback(selectedItem, Action::SelectionIn);
        }
    }
    else if (index == -1)
    {
        m_selectionIndex = -1;
    }
}

void UIList::HandleShortcuts()
{
    int newSelectedIndex = -1;

    int numItems = GetNumItems();
    if (Input::GetKeyDownRepeat(Key::Down) || Input::GetKeyDownRepeat(Key::Up))
    {
        int inc = Input::GetKeyDownRepeat(Key::Down) ? 1 : -1;
        GOItem *newSelectedItem;
        newSelectedIndex = GetSelectedIndex();
        do
        {
            newSelectedIndex = (newSelectedIndex + inc + numItems) % numItems;
            newSelectedItem = GetItem(newSelectedIndex);
            if (newSelectedIndex == GetSelectedIndex()) { break; }
        }
        while (newSelectedIndex != GetSelectedIndex() &&
               !newSelectedItem->IsEnabled());
    }
    else if (Input::GetKeyDownRepeat(Key::PageDown) ||
             Input::GetKeyDownRepeat(Key::PageUp))
    {
        int sign = Input::GetKeyDownRepeat(Key::PageDown) ? 1 : -1;
        GetScrollPanel()->SetScrolling( GetScrollPanel()->GetScrolling() +
                          sign * Vector2i(GetScrollPanel()->GetContainerSize()) );
    }
    else if (Input::GetKeyDown(Key::End)) { newSelectedIndex = GetNumItems() - 1; }
    else if (Input::GetKeyDown(Key::Home)) { newSelectedIndex = 0; }

    if (newSelectedIndex >= 0)
    {
        SetSelection(newSelectedIndex);
    }
}

void UIList::OnFocusTaken(IFocusable *focusable)
{
    IFocusListener::OnFocusTaken(focusable);
    m_someChildHasFocus = true;
}

void UIList::OnFocusLost(IFocusable *focusable)
{
    IFocusListener::OnFocusLost(focusable);
    m_someChildHasFocus = false;
}

void UIList::SetSelection(GOItem *item)
{
    SetSelection(p_items.IndexOf(item));
}

GameObject *UIList::GetContainer() const
{
    return p_container;
}

int UIList::GetSelectedIndex() const
{
    return m_selectionIndex;
}

bool UIList::SomeChildHasFocus() const { return m_someChildHasFocus; }

GOItem *UIList::GetSelectedItem() const
{
    return GetItem( GetSelectedIndex() );
}

void UIList::OnDestroyed(Object *object)
{
    if (object == p_itemUnderMouse)
    {
        if (m_selectionCallback) { m_selectionCallback(p_itemUnderMouse,
                                                       Action::SelectionOut); }
        p_itemUnderMouse = nullptr;
    }
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

    container->GetRectTransform()->SetPivotPosition(Vector2(-1,1));
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
