#ifndef UILIST_H
#define UILIST_H

#include <functional>

#include "Bang/Map.h"
#include "Bang/Component.h"
#include "Bang/IFocusListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIScrollArea;
FORWARD class UIScrollPanel;
FORWARD class UIImageRenderer;

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
    void AddItem(GOItem *newItem, int index);
    void RemoveItem(GOItem *item);
    void ClearSelection();
    void Clear();

    void SetOverColor(const Color &overColor);
    void SetUseSelectedColor(const Color &selectedColor);
    void SetUseSelectedColor(bool useSelectColor);

    const Array<GOItem*>& GetItems() const;
    GOItem *GetItem(int i) const;

    void ScrollTo(int i);
    void ScrollTo(GOItem *item);
    void SetSelection(int i);
    void SetSelection(GOItem *item);

    int GetNumItems() const;
    GameObject *GetContainer() const;
    UIScrollPanel *GetScrollPanel() const;
    const Color& GetOverColor() const;
    const Color& GetSelectedColor() const;

    bool SomeChildHasFocus() const;
    int GetSelectedIndex() const;
    GOItem* GetSelectedItem() const;

    void SetWideSelectionMode(bool wideSelectionMode);

    // IDestroyListener
    virtual void OnDestroyed(Object *object) override;

    using SelectionCallback = std::function<void(GOItem *item, Action action)>;
    void SetSelectionCallback(SelectionCallback selectionCallback);

protected:
    UIList();
    virtual ~UIList();

private:
    Array<GOItem*> p_items;
    Map<GOItem*, UIImageRenderer*> p_itemsBackground;
    bool m_someChildHasFocus = false;

    int m_selectionIndex = -1;
    GOItem *p_itemUnderMouse = nullptr;
    SelectionCallback m_selectionCallback;

    GameObject *p_container = nullptr;
    UIScrollPanel *p_scrollPanel = nullptr;

    bool m_useSelectColor = true;
    Color m_overColor = Color::VeryLightBlue;
    Color m_selectedColor = Color::LightBlue;

    bool m_wideSelectionMode = true;

    void HandleShortcuts();

    // IFocusListener
    virtual void OnFocusTaken(IFocusable *focusable) override;
    virtual void OnFocusLost(IFocusable *focusable) override;

    static UIList* CreateInto(GameObject *go, bool withScrollPanel);
    void CallSelectionCallback(GameObject *item, Action action);

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UILIST_H
