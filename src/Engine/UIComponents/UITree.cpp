#include "Bang/UITree.h"

#include "Bang/Input.h"
#include "Bang/UIButton.h"
#include "Bang/UICanvas.h"
#include "Bang/IconManager.h"
#include "Bang/RectTransform.h"
#include "Bang/UIButtoneable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

UITree::UITree()
{
    m_tree.SetData(nullptr);
}

UITree::~UITree()
{
}

void UITree::OnUpdate()
{
    Component::OnUpdate();

    // Collapse with left-right buttons
    if (UICanvas::HasFocus(this))
    {
        int collapseOnOff = 0;
        if (Input::GetKeyDownRepeat(Key::Left)) { collapseOnOff = -1; }
        else if (Input::GetKeyDownRepeat(Key::Right)) { collapseOnOff = 1; }
        if (collapseOnOff != 0)
        {
            GOItem *selItemCont = GetUIList()->GetSelectedItem();
            if (selItemCont)
            {
                UITreeItemContainer *selectedItemCont =
                                        Cast<UITreeItemContainer*>(selItemCont);
                GOItem *selectedItem = selectedItemCont->GetContainedItem();

                bool isCollapsed = selectedItemCont->IsCollapsed();
                int numChildren = GetItemTree(selectedItem)->GetChildren().Size();

                int newSelIndex = GetUIList()->GetSelectedIndex();

                if ( (numChildren == 0 || !isCollapsed) && collapseOnOff == 1)
                { ++newSelIndex; }

                else if ( (numChildren == 0 || isCollapsed) && collapseOnOff == -1)
                { --newSelIndex; }

                if (newSelIndex == GetUIList()->GetSelectedIndex())
                {
                    // Normal Collapse/UnCollapse
                    GOItem *selectedItem = selectedItemCont->GetContainedItem();
                    SetItemCollapsed(selectedItem, (collapseOnOff == -1) );
                }
                else
                {
                    // "Redundant" Collapse/UnCollapse. Go Up or Down
                    newSelIndex = Math::Clamp(newSelIndex,
                                              0, GetUIList()->GetNumItems()-1);
                    GetUIList()->SetSelection(newSelIndex);
                }
            }
        }
    }
}

void UITree::OnButton_Clicked(UIButtoneable *btn)
{
    UITreeItemContainer *itemContainer = GetItemContainer(btn->GetGameObject());
    SetItemCollapsed(itemContainer->GetContainedItem(),
                     !itemContainer->IsCollapsed());
}

void UITree::OnDestroyed(Object *object)
{
    GOItem* item = Cast<GOItem*>(object);
    RemoveItem(item);
}

List<GOItem*> UITree::GetChildrenItems(GOItem *item)
{
    List<GOItem*> childrenItems;
    if ( m_itemToTree.ContainsKey(item) || !item )
    {
        const Tree<GOItem*>* itemTree = item ? m_itemToTree.Get(item) : &m_tree;
        const List<Tree<GOItem*>*>& childrenTrees = itemTree->GetChildren();
        for (const Tree<GOItem*>* childTree : childrenTrees)
        {
            childrenItems.PushBack(childTree->GetData());
        }
    }
    return childrenItems;
}

void UITree::AddItem(GOItem *newItem, GOItem *parentItem)
{
    Tree<GOItem*> *parentTree = GetItemTree(parentItem);
    if (parentTree && !m_itemToTree.ContainsKey(newItem))
    {
        UITreeItemContainer *itemContainer =
                                GameObject::Create<UITreeItemContainer>();
        itemContainer->GetCollapseButton()->GetButton()
                ->EventEmitter<IUIButtonListener>::RegisterListener(this);
        itemContainer->SetContainedItem(newItem);

        Tree<GOItem*> *childTree = parentTree->AddChild(newItem);
        m_itemToTree.Add(newItem, childTree);
        GetUIList()->AddItem(itemContainer);
        newItem->EventEmitter<IDestroyListener>::RegisterListener(this);

        // Update collapsabilities
        UpdateCollapsability(newItem);
        if (parentItem) { UpdateCollapsability(parentItem); }

        IndentItem(newItem); // Indent
    }
    else
    {
        Debug_Warn("Can't add item " << newItem << " to " << parentItem <<
                   " because it does not exist");
    }
}

void UITree::RemoveItem(GOItem *item)
{
    Tree<GOItem*> *itemTree = GetItemTree(item);
    if (itemTree)
    {
        GOItem* parentItem = GetParentItem(item); // Get parent before deleting

        // Finally remove from list, remove from map, and delete the item tree
        Tree<GOItem*> *itemTree = GetItemTree(item);
        m_itemToTree.Remove( item ); // Must go before list item removal
        delete itemTree;

        GetUIList()->RemoveItem( GetItemContainer(item) );

        // Update parent collapsability
        if (parentItem) { UpdateCollapsability(parentItem); }
    }
}

GOItem *UITree::GetSelectedItem() const
{
    GOItem *selectedItem = GetUIList()->GetSelectedItem();
    if (selectedItem)
    {
        UITreeItemContainer *selectedItemCont =
                                Cast<UITreeItemContainer*>(selectedItem);
        return selectedItemCont->GetContainedItem();
    }
    return nullptr;
}

void UITree::Clear()
{
    GetUIList()->Clear();
    m_tree.Clear();
    m_itemToTree.Clear();
}

void UITree::SetSelection(GOItem *item)
{
    UnCollapseUpwards( GetParentItem(item) );
    UITreeItemContainer *itemContainer = GetItemContainer(item);
    if (itemContainer)
    {
        itemContainer->SetEnabled(true);
        SetItemCollapsed(item, itemContainer->IsCollapsed());
    }
    GetUIList()->SetSelection(itemContainer);
}

void UITree::SetItemCollapsed(GOItem *item, bool collapse)
{
    if (!collapse) { UnCollapseUpwards(item); }

    GetItemContainer(item)->SetCollapsed(collapse);
    _SetItemCollapsedRec(item, collapse);
}

void UITree::_SetItemCollapsedRec(GOItem *item, bool collapse)
{
    List<GOItem*> childrenItems = GetChildrenItems(item);
    for (GOItem *childItem : childrenItems)
    {
        UITreeItemContainer *childItemContainer = GetItemContainer(childItem);
        childItemContainer->SetEnabled(!collapse);
        _SetItemCollapsedRec(childItem,
                            (collapse || childItemContainer->IsCollapsed()));
    }
}

void UITree::UnCollapseUpwards(GOItem *item)
{
    ENSURE(item);

    UITreeItemContainer *itemContainer = GetItemContainer(item);
    itemContainer->SetEnabled(true);
    itemContainer->SetCollapsed(false);
    UnCollapseUpwards( GetParentItem(item) );
}

void UITree::SetSelectionCallback(UIList::SelectionCallback callback)
{
    m_selectionCallback = callback;
}

bool UITree::IsItemCollapsed(GOItem *item) const
{
    return GetItemContainer(item)->IsCollapsed();
}

UIList *UITree::GetUIList() const { return p_uiList; }

UITree *UITree::CreateInto(GameObject *go)
{
    UIList *list = GameObjectFactory::CreateUIListInto(go);

    UITree *uiTree = go->AddComponent<UITree>();
    uiTree->p_uiList = list;

    uiTree->GetUIList()->SetSelectionCallback(
        [uiTree](GOItem *item, UIList::Action action)
        {
            // Forward selectionCallback from itemContainer to
            // actual item
            if (uiTree->m_selectionCallback)
            {
                UITreeItemContainer *itemCont = Cast<UITreeItemContainer*>(item);
                uiTree->m_selectionCallback( itemCont->GetContainedItem(), action);
            }
        }
    );

    return uiTree;
}

UITreeItemContainer *UITree::GetItemContainer(GOItem *item) const
{
    ASSERT(!item || !Cast<UITreeItemContainer*>(item));
    return item ? Cast<UITreeItemContainer*>(item->GetParent()) : nullptr;
}

void UITree::UpdateCollapsability(GOItem *item)
{
    Tree<GOItem*> *itemTree = GetItemTree(item);
    if (itemTree)
    {
        bool isLeaf = (itemTree->GetChildren().Size() == 0);
        GetItemContainer(item)->SetCollapsable(!isLeaf);
    }
}

void UITree::IndentItem(GOItem *item)
{
    Tree<GOItem*> *itemTree = GetItemTree(item);
    GetItemContainer(item)->SetIndentation(10 * itemTree->GetDepth());
}

GOItem* UITree::GetParentItem(GOItem *item)
{
    Tree<GOItem*>* itemTree = GetItemTree(item);
    if (!itemTree) { return nullptr; }

    Tree<GOItem*>* parentTree = itemTree ? itemTree->GetParent() : nullptr;
    if (!parentTree) { return nullptr; }

    return (parentTree != &m_tree) ? parentTree->GetData() : nullptr;
}

Tree<GOItem*>* UITree::GetItemTree(GOItem *item)
{
    if (!item) { return &m_tree; }

    // Make sure item belongs to this UITree
    if (m_itemToTree.ContainsKey(item))  { return m_itemToTree.Get(item); }
    return nullptr;
}

// UITreeItemContainer
UITreeItemContainer::UITreeItemContainer()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    AddComponent<UIHorizontalLayout>();
    p_spacer = GameObjectFactory::CreateUISpacer(LayoutSizeType::Preferred,
                                                 Vector2i::Zero);

    p_collapseButton = GameObjectFactory::CreateUIButton();
    p_collapseButton->SetIcon(IconManager::GetDownArrowIcon(), Vector2i(8), 0);
    p_collapseButton->GetBackground()->SetVisible(false);    

    SetAsChild(p_spacer);
    SetAsChild(p_collapseButton->GetGameObject());
}

UITreeItemContainer::~UITreeItemContainer()
{

}

void UITreeItemContainer::SetCollapsable(bool collapsable)
{
    GetCollapseButton()->SetEnabled(collapsable);
    GetCollapseButton()->GetIcon()->SetVisible(collapsable);
    GetCollapseButton()->GetButton()->SetEnabled(collapsable);
}

void UITreeItemContainer::SetCollapsed(bool collapsed)
{
    if (collapsed != IsCollapsed())
    {
        m_collapsed = collapsed;
        GetCollapseButton()->SetIconTexture(
                    IsCollapsed() ? IconManager::GetRightArrowIcon() :
                                    IconManager::GetDownArrowIcon() );
    }
}

void UITreeItemContainer::SetContainedItem(GameObject *go)
{
    p_containedGameObject = go;
    SetAsChild(p_containedGameObject);
}

GameObject *UITreeItemContainer::GetContainedItem() const
{
    return p_containedGameObject;
}

void UITreeItemContainer::SetIndentation(int indentationPx)
{
    p_spacer->GetComponent<UILayoutElement>()->SetPreferredWidth(indentationPx);
}

bool UITreeItemContainer::IsCollapsed() const
{
    return m_collapsed;
}

UIButton *UITreeItemContainer::GetCollapseButton() const
{
    return p_collapseButton;
}
