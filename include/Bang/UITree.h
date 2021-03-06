#ifndef UITREE_H
#define UITREE_H

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/Tree.h"
#include "Bang/UIList.h"
#include "Bang/UIButton.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/IFocusListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class UITreeItemContainer;

class UITree : public Component,
               public IFocusListener,
               public IDestroyListener
{
    COMPONENT(UITree)

public:

    void AddItem(GOItem *newItem, GOItem *parentItem, int indexInsideParent = 0);
    void RemoveItem(GOItem *itemToRemove);
    GOItem* GetSelectedItem() const;
    void Clear();

    void SetSelection(GOItem *item);
    void SetItemCollapsed(GOItem *item, bool collapsed);
    void SetSelectionCallback(UIList::SelectionCallback callback);

    GOItem* GetParentItem(GOItem *item);
    bool IsItemCollapsed(GOItem *item) const;
    List<GOItem*> GetChildrenItems(GOItem *item);
    UIList* GetUIList() const;

    // Component
    void OnUpdate() override;

    // IFocusListener
    void OnClicked(IFocusable *focusable) override;

    // IDestroyListener
    void OnDestroyed(EventEmitter<IDestroyListener> *object) override;

protected:
    UITree();
    virtual ~UITree();

    static UITree* CreateInto(GameObject *go);

private:
    Tree<GOItem*> m_tree;
    UIList *p_uiList = nullptr;
    Map<GOItem*, Tree<GOItem*>*> m_itemToTree;
    UIList::SelectionCallback m_selectionCallback;

    void UnCollapseUpwards(GOItem *item);
    void _SetItemCollapsedRec(GOItem *item, bool collapse);
    void UpdateCollapsability(GOItem *item);
    void IndentItem(GOItem *item);

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
    GameObject *p_indentSpacer = nullptr;
};

NAMESPACE_BANG_END

#endif // UITREE_H

