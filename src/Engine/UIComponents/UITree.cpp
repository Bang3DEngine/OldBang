#include "Bang/UITree.h"

#include "Bang/Input.h"
#include "Bang/IconManager.h"
#include "Bang/RectTransform.h"
#include "Bang/UITintedButton.h"
#include "Bang/UIButtonDriver.h"
#include "Bang/UITintedButton.h"
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

    for (auto &it : m_itemToTree)
    {
        GOItem *child = it.first;
        Tree<GOItem*> *tree = it.second;
        IndentItem(child, tree->GetDepth()-1);
    }
}

void UITree::OnButton_Clicked(UIButton *btn)
{
    UITreeItemContainer *itemContainer = GetItemContainer(btn->GetGameObject());
    SetItemCollapsed(itemContainer->GetContainedItem(),
                     !itemContainer->IsCollapsed());
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

        // Update collapsabilities
        UpdateCollapsability(newItem);
        if (parentItem) { UpdateCollapsability(parentItem); }
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
        GetUIList()->RemoveItem( GetItemContainer(item) );
        m_itemToTree.Remove( item );
        delete itemTree;

        // Update parent collapsability
        if (parentItem) { UpdateCollapsability(parentItem); }
    }
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
    if (itemContainer) { itemContainer->SetEnabled(true); }
    GetUIList()->SetSelection(itemContainer);
}

void UITree::SetItemCollapsed(GOItem *item, bool collapsed)
{
    if (!collapsed) { UnCollapseUpwards(item); }
    _SetItemCollapsedRec(item, collapsed, true);
}

void UITree::_SetItemCollapsedRec(GOItem *item, bool collapse, bool firstCall)
{
    if (firstCall) { GetItemContainer(item)->SetCollapsed(collapse); }

    List<GOItem*> childrenItems = GetChildrenItems(item);
    for (GOItem *childItem : childrenItems)
    {
        UITreeItemContainer *childItemContainer = GetItemContainer(childItem);
        childItemContainer->SetEnabled(!collapse);
        _SetItemCollapsedRec(childItem,
                            (collapse || childItemContainer->IsCollapsed()),
                            false);
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

    uiTree->()->SetSelectionCallback(
        [uiTree](GOItem *item, UIList::Action action)
        {
            // Forward selectionCallback from itemContainer to
            // actual item
            if (uiTree->m_selectionCallback)
            {
                UITreeItemContainer *itemCont = SCAST<UITreeItemContainer*>(item);
                uiTree->m_selectionCallback( itemCont->GetContainedItem(), action);
            }
        }
    );

    return uiTree;
}

UITreeItemContainer *UITree::GetItemContainer(GOItem *item) const
{
    return SCAST<UITreeItemContainer*>(item->GetParent());
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

void UITree::IndentItem(GOItem *child, int indentation)
{
    GetItemContainer(child)->SetIndentation(10 * indentation);
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

UIButtonDriver *UITreeItemContainer::GetCollapseButton() const
{
    return p_collapseButton;
}
