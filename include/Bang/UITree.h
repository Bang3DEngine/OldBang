#ifndef UITREE_H
#define UITREE_H

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/Tree.h"
#include "Bang/UIList.h"
#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

class UITree : public IComponentDriver<UITree>,
               public Component
{
    COMPONENT(UITree);

public:

    void AddItem(GOItem *newItem, GOItem *parentItem);
    void RemoveItem(GOItem *itemToRemove);
    void Clear();

    void SetSelectionCallback(UIList::SelectionCallback callback);

    // Component
    void OnUpdate() override;

    List<GOItem*> GetChildrenItems(GOItem *item);
    UIList* GetUIList() const;

protected:
    UITree();
    virtual ~UITree();

    static UITree* CreateInto(GameObject *go);

private:
    UIList *p_uiList = nullptr;
    Tree<GOItem*> m_tree;
    Map<GOItem*, Tree<GOItem*>*> m_itemToTree;
    UIList::SelectionCallback m_selectionCallback;

    void IndentItem(GOItem *item, int indentation);

    friend class GameObjectFactory;
    friend class IComponentDriver<UITree>;
};

class UITreeItemContainer : public GameObject
{
public:
    UITreeItemContainer();
    virtual ~UITreeItemContainer();

    void SetContainedItem(GOItem *item);

    GOItem *GetContainedItem() const;
    void SetIndentation(int indentationPx);

private:
    GOItem *p_containedGameObject = nullptr;
    GameObject *p_spacer = nullptr;
};

NAMESPACE_BANG_END

#endif // UITREE_H

