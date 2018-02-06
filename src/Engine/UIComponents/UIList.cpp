#include "Bang/UIList.h"

#include "Bang/Input.h"
#include "Bang/AARect.h"
#include "Bang/Material.h"
#include "Bang/UICanvas.h"
#include "Bang/IFocusable.h"
#include "Bang/GameObject.h"
#include "Bang/UIFocusable.h"
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

    // Mouse In/Out
    UICanvas *canvas = UICanvas::GetActive(this);
    GOItem *itemUnderMouse = nullptr;
    if (canvas->IsMouseOver(GetContainer(), true))
    {
        for (GOItem *childItem : p_items)
        {
            if (!childItem->IsEnabled()) { continue; }

            bool overChildItem = false;
            overChildItem = canvas->IsMouseOver(childItem, true);
            if (m_wideSelectionMode && !overChildItem)
            {
                Vector2 mousePos = Input::GetMousePositionNDC();
                AARect listRTRect ( GetGameObject()->GetRectTransform()->GetViewportRectNDC() );
                AARect itemRTRect ( childItem->GetRectTransform()->GetViewportRectNDC() );
                overChildItem = (mousePos.x >= listRTRect.GetMin().x &&
                                 mousePos.x <= listRTRect.GetMax().x &&
                                 mousePos.y >= itemRTRect.GetMin().y &&
                                 mousePos.y <= itemRTRect.GetMax().y);
            }

            if (overChildItem)
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
            CallSelectionCallback(p_itemUnderMouse, Action::MouseOut);
        }

        p_itemUnderMouse = itemUnderMouse;
        if (p_itemUnderMouse)
        {
            CallSelectionCallback(p_itemUnderMouse, Action::MouseOver);
        }
    }

    if (!p_itemUnderMouse)
    {
        if (p_itemUnderMouse) { CallSelectionCallback(p_itemUnderMouse, Action::MouseOut); }
        p_itemUnderMouse = nullptr;
    }

    bool someChildHasFocus = m_wideSelectionMode ? (p_itemUnderMouse != nullptr) :
                                                    SomeChildHasFocus();
    if (someChildHasFocus)
    {
        HandleShortcuts();

        // Clicked
        if (Input::GetKeyDownRepeat(Key::Right) ||
            Input::GetKeyDownRepeat(Key::Enter))
        {
            GOItem *selectedItem = GetSelectedItem();
            if (selectedItem) { CallSelectionCallback(selectedItem, Action::Pressed); }
        }

        if (p_itemUnderMouse)
        {
            if (Input::GetMouseButtonDown(MouseButton::Left))
            {
                SetSelection(p_itemUnderMouse);
                CallSelectionCallback(p_itemUnderMouse, Action::ClickedLeft);
            }

            if (Input::GetMouseButtonDown(MouseButton::Right))
            {
                CallSelectionCallback(p_itemUnderMouse, Action::ClickedRight);
            }

            if (Input::GetMouseButtonDoubleClick(MouseButton::Left))
            {
                CallSelectionCallback(p_itemUnderMouse, Action::DoubleClickedLeft);
            }
        }
    }
}

void UIList::AddItem(GOItem *newItem)
{
    AddItem(newItem, GetNumItems());
}

void UIList::AddItem(GOItem *newItem, int index)
{
    ASSERT(index >= 0 && index <= GetNumItems());

    List<IFocusable*> newItemFocusables =
                            newItem->GetComponentsInChildren<IFocusable>(true);

    UIImageRenderer *itemBg = newItem->AddComponent<UIImageRenderer>(0);
    itemBg->SetTint( GetIdleColor() );

    for (IFocusable* newItemFocusable : newItemFocusables)
    {
        newItemFocusable->EventEmitter<IFocusListener>::RegisterListener(this);
    }

    newItem->EventEmitter<IDestroyListener>::RegisterListener(this);
    newItem->SetParent(GetContainer(), index);

    p_itemsBackground.Add(newItem, itemBg);
    p_items.Insert(newItem, index);

    if (index <= m_selectionIndex) { ++m_selectionIndex; }
}

void UIList::RemoveItem(GOItem *item)
{
    ASSERT( p_items.Contains(item) );

    int indexOfItem = p_items.IndexOf(item);

    if (p_itemUnderMouse == item) { p_itemUnderMouse = nullptr; }
    if (indexOfItem < GetSelectedIndex()) { m_selectionIndex -= 1; }
    if (GetSelectedIndex() == indexOfItem) { ClearSelection(); }

    // Destroy the element
    GameObject::Destroy(item);
    p_items.Remove(item);
    p_itemsBackground.Remove(item);
}

void UIList::ClearSelection()
{
    if (GetSelectedIndex() >= 0)
    {
        SetSelection(-1);
    }
}

void UIList::Clear()
{
    while (!p_items.IsEmpty()) { RemoveItem(p_items.Front()); }
    if (GetScrollPanel())
    {
        GetScrollPanel()->SetScrollingPercent( Vector2(0.0f) );
    }
    ClearSelection();
}

void UIList::SetIdleColor(const Color &idleColor)
{
    m_idleColor = idleColor;
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
    if (!GetScrollPanel()) { return; }

    AARect itemRect ( item->GetRectTransform()-> GetViewportRect() );
    AARect panelRect ( GetScrollPanel()->GetGameObject()->GetRectTransform()->
                                                          GetViewportRect() );
    AARect containerRect ( GetContainer()->GetRectTransform()-> GetViewportRect() );

    Vector2 relativeItemRectMin = itemRect.GetMin() - containerRect.GetMin();
    relativeItemRectMin.y = (containerRect.GetHeight() - relativeItemRectMin.y);
    Vector2 relativeItemRectMax = relativeItemRectMin + itemRect.GetSize();
    AARect relativeItemRect;
    relativeItemRect.SetMin( Vector2::Min(relativeItemRectMin, relativeItemRectMax) );
    relativeItemRect.SetMax( Vector2::Max(relativeItemRectMin, relativeItemRectMax) );

    Vector2i scrolling = -Vector2i::One;
    if (itemRect.GetMax().y > panelRect.GetMax().y)
    {
        scrolling = Vector2i(relativeItemRect.GetMax() - panelRect.GetHeight());
    }
    else if (itemRect.GetMin().y < panelRect.GetMin().y)
    {
        scrolling = Vector2i(relativeItemRect.GetMin() - panelRect.GetHeight());
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

UIDirLayout *UIList::GetDirLayout() const
{
    return GetGameObject()->GetComponent<UIDirLayout>();
}

void UIList::SetSelection(int index)
{
    if (GetSelectedIndex() != index)
    {
        GOItem *prevSelectedItem = GetSelectedItem();
        if (prevSelectedItem) { CallSelectionCallback(prevSelectedItem, Action::SelectionOut); }
    }

    if (GetSelectedIndex() != index && index >= 0 && index < GetNumItems())
    {
        m_selectionIndex = index;
        GOItem *selectedItem = GetSelectedItem();
        if (selectedItem)
        {
            ScrollTo(selectedItem);

            IFocusable *itemFocusable = selectedItem->
                                        GetComponentInChildren<IFocusable>(true);
            UICanvas *canvas = GetGameObject()->GetComponentInParent<UICanvas>();
            if (canvas) { canvas->SetFocus(itemFocusable); }

            CallSelectionCallback(selectedItem, Action::SelectionIn);
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
        if (GetScrollPanel())
        {
            int sign = Input::GetKeyDownRepeat(Key::PageDown) ? 1 : -1;
            GetScrollPanel()->SetScrolling( GetScrollPanel()->GetScrolling() +
                              sign * Vector2i(GetScrollPanel()->GetContainerSize()) );
        }
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

void UIList::SetWideSelectionMode(bool wideSelectionMode)
{
    m_wideSelectionMode = wideSelectionMode;
}

void UIList::SetOverColor(const Color &overColor)
{
    m_overColor = overColor;
}

void UIList::SetSelectedColor(const Color &selectedColor)
{
    m_selectedColor = selectedColor;
}

void UIList::SetUseSelectedColor(bool useSelectColor)
{
    m_useSelectColor = useSelectColor;
}

void UIList::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    if (object == p_itemUnderMouse)
    {
        CallSelectionCallback(p_itemUnderMouse, Action::SelectionOut);
        p_itemUnderMouse = nullptr;
    }
}

void UIList::SetSelectionCallback(SelectionCallback selectionCallback)
{
    m_selectionCallback = selectionCallback;
}

UIList* UIList::CreateInto(GameObject *go, bool withScrollPanel)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UIList *list = go->AddComponent<UIList>();

    const bool vertical = true;
    GameObject *container = withScrollPanel ?
                                GameObjectFactory::CreateUIGameObject() : go;

    UIDirLayout *dirLayout;
    if (vertical) { dirLayout = container->AddComponent<UIVerticalLayout>(); }
    else { dirLayout = container->AddComponent<UIHorizontalLayout>(); }
    (void) dirLayout;

    UIFocusable *focusable = container->AddComponent<UIFocusable>();
    (void)(focusable);

    if (withScrollPanel)
    {
        UIScrollPanel *scrollPanel = nullptr;
        scrollPanel = GameObjectFactory::CreateUIScrollPanelInto(go);

        UIContentSizeFitter *csf = container->AddComponent<UIContentSizeFitter>();
        csf->SetHorizontalSizeType(LayoutSizeType::Preferred);
        csf->SetVerticalSizeType(LayoutSizeType::Preferred);
        container->GetRectTransform()->SetPivotPosition(Vector2(-1,1));

        scrollPanel->GetScrollArea()->SetContainedGameObject(container);

        list->p_scrollPanel = scrollPanel;
    }
    else { list->p_scrollPanel = nullptr; }

    list->p_container = container;

    return list;
}

void UIList::CallSelectionCallback(GOItem *item, Action action)
{
    UIImageRenderer *itemBg = p_itemsBackground.Get(item);
    ASSERT(itemBg);

    bool isSelected = (GetSelectedItem() == item);
    switch (action)
    {
        case UIList::Action::MouseOver:
            if (!m_useSelectColor || !isSelected) { itemBg->SetTint( GetOverColor() ); }
        break;

        case UIList::Action::MouseOut:
            if (!m_useSelectColor || !isSelected) { itemBg->SetTint( GetIdleColor() ); }
        break;

        case UIList::Action::SelectionIn:
            if (m_useSelectColor) { itemBg->SetTint( GetSelectedColor() ); }
        break;

        case UIList::Action::SelectionOut:
            if (m_useSelectColor) { itemBg->SetTint( GetIdleColor() ); }
        break;

        default: break;
    }

    if (m_selectionCallback) { m_selectionCallback(item, action); }
}

UIScrollPanel *UIList::GetScrollPanel() const { return p_scrollPanel; }

const Color &UIList::GetIdleColor() const
{
    return m_idleColor;
}

const Color &UIList::GetOverColor() const
{
    return m_overColor;
}

const Color &UIList::GetSelectedColor() const
{
    return m_selectedColor;
}
