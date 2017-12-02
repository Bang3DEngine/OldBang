#ifndef UILIST_H
#define UILIST_H

#include <functional>
#include <unordered_map>

#include "Bang/Component.h"
#include "Bang/IFocusListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIScrollArea;
FORWARD class UIScrollPanel;

using GOItem = GameObject;

class UIList : public Component,
               public IFocusListener,
               public IDestroyListener
{
    COMPONENT(UIList)

public:
    enum Action { SelectionIn, SelectionOut, MouseOver, MouseOut,
                  Pressed, DoubleClickedLeft, ClickedLeft, ClickedRight };

    // Component
    void OnUpdate() override;

    void AddItem(GOItem *newItem);
    void RemoveItem(GOItem *item);
    void ClearSelection();
    void Clear();

    const Array<GOItem*>& GetItems() const;
    GOItem *GetItem(int i) const;

    void ScrollTo(int i);
    void ScrollTo(GOItem *item);
    void SetSelection(int i);
    void SetSelection(GOItem *item);

    int GetNumItems() const;
    GameObject *GetContainer() const;
    UIScrollPanel *GetScrollPanel() const;

    bool SomeChildHasFocus() const;
    int GetSelectedIndex() const;
    GOItem* GetSelectedItem() const;

    // IDestroyListener
    virtual void OnDestroyed(Object *object) override;

    using SelectionCallback = std::function<void(GOItem *item, Action action)>;
    void SetSelectionCallback(SelectionCallback selectionCallback);

protected:
    UIList();
    virtual ~UIList();

private:
    Array<GOItem*> p_items;
    bool m_someChildHasFocus = false;

    int m_selectionIndex = -1;
    GOItem *p_itemUnderMouse = nullptr;
    SelectionCallback m_selectionCallback;

    GameObject *p_container = nullptr;
    UIScrollPanel *p_scrollPanel = nullptr;

    void HandleShortcuts();

    // IFocusListener
    virtual void OnFocusTaken(IFocusable *focusable) override;
    virtual void OnFocusLost(IFocusable *focusable) override;

    static UIList* CreateInto(GameObject *go);
    void Callback(GameObject *item, Action action);

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UILIST_H
