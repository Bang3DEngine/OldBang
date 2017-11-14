#include "Bang/UITree.h"

#include "Bang/Input.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

UITree::UITree()
{
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

List<GOItem*> UITree::GetChildrenItems(GOItem *item)
{
    List<GOItem*> childrenItems;
    if ( m_itemToTree.ContainsKey(item) || !item )
    {
        const Tree<GOItem*>* itemTree = item ? m_itemToTree.Get(item) : &m_tree;
        const List<Tree<GOItem*>>& childrenTrees = itemTree->GetChildren();
        for (const Tree<GOItem*> &childTree : childrenTrees)
        {
            childrenItems.PushBack(childTree.GetData());
        }
    }
    return childrenItems;
}

void UITree::AddItem(GOItem *newItem, GOItem *parentItem)
{
    Tree<GOItem*> *parentTree = nullptr;
    if (parentItem)
    {
        if (m_itemToTree.ContainsKey(parentItem))
        {
            parentTree = m_itemToTree.Get(parentItem);
        }
    }
    else { parentTree = &m_tree; }

    if (parentTree)
    {
        GameObject *itemContainer = ObjectManager::Create<UITreeItemContainer>();
        itemContainer->SetAsChild(newItem);

        Tree<GOItem*> *childTree = parentTree->AddChild(newItem);
        m_itemToTree.Add(newItem, childTree);
        GetUIList()->AddItem(itemContainer);
    }
    else
    {
        Debug_Warn("Can't add item " << newItem << " to " << parentItem <<
                   " because it does not exist");
    }
}

void UITree::RemoveItem(GOItem *item)
{
    if (m_itemToTree.ContainsKey(item))
    {
        Tree<GOItem*> *parentTree = m_itemToTree.Get(item);
        delete parentTree;

        GetUIList()->RemoveItem( item->GetParent() );
        m_itemToTree.Remove( item );
    }
}

void UITree::Clear()
{
    GetUIList()->Clear();
    m_tree.Clear();
    m_itemToTree.Clear();
}

UIList *UITree::GetUIList() const { return p_uiList; }

UITree *UITree::CreateInto(GameObject *go)
{
    UIList *list = GameObjectFactory::CreateUIListInto(go);

    UITree *tree = go->AddComponent<UITree>();
    tree->p_uiList = list;

    return tree;
}

void UITree::IndentItem(GOItem *child, int indentation)
{
    UITreeItemContainer *itemContainer =
                            DCAST<UITreeItemContainer*>(child->GetParent());
    ASSERT(itemContainer);

    itemContainer->SetIndentation(10 * indentation);
}

// UITreeItemContainer
UITreeItemContainer::UITreeItemContainer()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    AddComponent<UIHorizontalLayout>();
    p_spacer = GameObjectFactory::CreateUISpacer(LayoutSizeType::Preferred,
                                                 Vector2i::Zero);
    SetAsChild(p_spacer);
}

UITreeItemContainer::~UITreeItemContainer()
{

}

void UITreeItemContainer::SetIndentation(int indentationPx)
{
    p_spacer->GetComponent<UILayoutElement>()->SetPreferredWidth(indentationPx);
}
