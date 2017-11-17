#ifndef UITREE_H
#define UITREE_H

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/Tree.h"
#include "Bang/UIList.h"
#include "Bang/UIButton.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/UIButtoneable.h"

NAMESPACE_BANG_BEGIN

FORWARD class UITreeItemContainer;

class UITree : public Component,
               public IUIButtonListener
{
    COMPONENT(UITree);

public:

    void AddItem(GOItem *newItem, GOItem *parentItem);
    void RemoveItem(GOItem *itemToRemove);
    void Clear();

    void SetSelection(GOItem *item);
    void SetItemCollapsed(GOItem *item, bool collapsed);
    void SetSelectionCallback(UIList::SelectionCallback callback);

    bool IsItemCollapsed(GOItem *item) const;
    List<GOItem*> GetChildrenItems(GOItem *item);
    UIList* GetUIList() const;

    // Component
    void OnUpdate() override;

    // IUIButtonListener
    void OnButton_Clicked(UIButtoneable *btn) override;

protected:
    UITree();
    virtual ~UITree();

    static UITree* CreateInto(GameObject *go);

private:
    UIList *p_uiList = nullptr;
    Tree<GOItem*> m_tree;
    Map<GOItem*, Tree<GOItem*>*> m_itemToTree;
    UIList::SelectionCallback m_selectionCallback;

    void UnCollapseUpwards(GOItem *item);
    void _SetItemCollapsedRec(GOItem *item, bool collapse);
    void UpdateCollapsability(GOItem *item);
    void IndentItem(GOItem *item);

    GOItem* GetParentItem(GOItem *item);
    Tree<GOItem*>* GetItemTree(GOItem* item);
    UITreeItemContainer* GetItemContainer(GOItem *item) const;

    friend class GameObjectFactory;
};


// UITreeItemContainer
class UITreeItemContainer : public GameObject
{
public:
    UITreeItemContainer();
    virtual ~UITreeItemContainer();

    void SetCollapsable(bool collapsable);
    void SetCollapsed(bool collapsed);
    void SetContainedItem(GOItem *item);
    void SetIndentation(int indentationPx);

    bool IsCollapsed() const;
    GOItem *GetContainedItem() const;
    UIButton *GetCollapseButton() const;

private:
    bool m_collapsed = false;

    UIButton *p_collapseButton = nullptr;
    GOItem *p_containedGameObject = nullptr;
    GameObject *p_spacer = nullptr;
};

NAMESPACE_BANG_END

#endif // UITREE_H

