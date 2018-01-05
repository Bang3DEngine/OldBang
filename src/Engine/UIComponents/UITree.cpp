#include "Bang/UITree.h"

#include "Bang/Input.h"
#include "Bang/UIButton.h"
#include "Bang/UICanvas.h"
#include "Bang/IconManager.h"
#include "Bang/RectTransform.h"
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
    if (GetUIList()->SomeChildHasFocus())
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

void UITree::OnClicked(IFocusable *focusable)
{
    IFocusable *collapseButton = focusable;
    Component *cCollapseButton = Cast<Component*>(collapseButton);
    UITreeItemContainer *itemContainer =
               SCAST<UITreeItemContainer*>(cCollapseButton->GetGameObject()->
                                           GetParent());
    if (itemContainer)
    {
        SetItemCollapsed(itemContainer->GetContainedItem(),
                         !itemContainer->IsCollapsed());
    }
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

void UITree::AddItem(GOItem *newItem, GOItem *parentItem, int indexInsideParent)
{
    Tree<GOItem*> *parentTree = GetItemTree(parentItem);
    if (parentTree && !m_itemToTree.ContainsKey(newItem))
    {
        // Create itemContainer and populate
        auto newItemContainer = GameObject::Create<UITreeItemContainer>();
        newItemContainer->SetContainedItem(newItem);

        // Determine flat index in UIList
        UITreeItemContainer *parentItemContainer = GetItemContainer(parentItem);
        int parentItemIndex = GetUIList()->GetItems().IndexOf(parentItemContainer);
        int newItemFlatListIndex;
        if (indexInsideParent >  0 &&
            indexInsideParent < int(parentTree->GetChildren().Size()))
        {
            auto it = parentTree->GetChildren().Begin();
            std::advance(it, indexInsideParent - 1);
            GOItem* siblingItem = (*it)->GetData();
            Tree<GOItem*> *siblingTree = GetItemTree(siblingItem);
            UITreeItemContainer *siblingItemCont = GetItemContainer(siblingItem);

            newItemFlatListIndex = GetUIList()->GetItems().IndexOf(siblingItemCont);
            ASSERT(newItemFlatListIndex >= 0);
            ++newItemFlatListIndex;
            newItemFlatListIndex += siblingTree->GetChildrenRecursive().Size();
        }
        else
        {
            newItemFlatListIndex = parentItemIndex + 1;
        }

        // Add
        ASSERT(indexInsideParent >= 0);
        Tree<GOItem*> *childTree = parentTree->AddChild(newItem, indexInsideParent);
        m_itemToTree.Add(newItem, childTree); // Add to item_tree map
        GetUIList()->AddItem(newItemContainer, newItemFlatListIndex); // Add to UIList

        // Listen to focus and destroying
        newItemContainer->GetCollapseButton()->GetFocusable()
                ->EventEmitter<IFocusListener>::RegisterListener(this);
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
        // Get some info before deleting
        GOItem* parentItem = GetParentItem(item);

        List< Tree<GOItem*>* > treeChildrenRec = itemTree->GetChildrenRecursive();
        treeChildrenRec.PushBack(itemTree);

        List<GOItem*> itemsToRemove;
        for (Tree<GOItem*>* treeChild : treeChildrenRec)
        {
            itemsToRemove.PushBack( treeChild->GetData() );
        }

        // Remove needed stuff from list, remove from map, and delete the item
        // tree (which recursively deletes its children)
        for (GOItem* itemToRemove : itemsToRemove)
        {
            m_itemToTree.Remove(itemToRemove);
            GetUIList()->RemoveItem( GetItemContainer(itemToRemove) );
        }
        delete itemTree;

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
    p_indentSpacer = GameObjectFactory::CreateUISpacer(LayoutSizeType::Preferred,
                                                       Vector2::Zero);
    p_indentSpacer->SetName("IndentSpacer");

    p_collapseButton = GameObjectFactory::CreateUIButton();
    p_collapseButton->GetGameObject()->SetName("CollapseButton");
    RH<Texture2D> iconTex = IconManager::GetDownArrowIcon();
    p_collapseButton->SetIcon(iconTex.Get(), Vector2i(8), 0);
    p_collapseButton->GetBackground()->SetVisible(false);    

    p_indentSpacer->SetParent(this);
    p_collapseButton->GetGameObject()->SetParent(this);
}

UITreeItemContainer::~UITreeItemContainer()
{

}

void UITreeItemContainer::SetCollapsable(bool collapsable)
{
    GetCollapseButton()->SetEnabled(collapsable);
    GetCollapseButton()->GetIcon()->SetVisible(collapsable);
    GetCollapseButton()->GetFocusable()->SetEnabled(collapsable);
}

void UITreeItemContainer::SetCollapsed(bool collapsed)
{
    if (collapsed != IsCollapsed())
    {
        m_collapsed = collapsed;
        RH<Texture2D> iconTex = IsCollapsed() ? IconManager::GetRightArrowIcon() :
                                                  IconManager::GetDownArrowIcon();
        GetCollapseButton()->SetIconTexture(iconTex.Get());
    }
}

void UITreeItemContainer::SetContainedItem(GameObject *go)
{
    p_containedGameObject = go;
    p_containedGameObject->SetParent(this);
}

GameObject *UITreeItemContainer::GetContainedItem() const
{
    return p_containedGameObject;
}

void UITreeItemContainer::SetIndentation(int indentationPx)
{
    p_indentSpacer->GetComponent<UILayoutElement>()->SetPreferredWidth(indentationPx);
}

bool UITreeItemContainer::IsCollapsed() const
{
    return m_collapsed;
}

UIButton *UITreeItemContainer::GetCollapseButton() const
{
    return p_collapseButton;
}
