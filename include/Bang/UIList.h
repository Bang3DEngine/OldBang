#ifndef UILIST_H
#define UILIST_H

#include <functional>

#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIScrollArea;

using GOItem = GameObject;

class UIList : public IComponentDriver<UIList>,
               public Component
{
    COMPONENT(UIList);

public:
    enum Action { SelectionIn, SelectionOut, MouseOver, MouseOut,
                  Pressed, DoubleClickedLeft, ClickedLeft, ClickedRight };

    void OnUpdate() override;
    void AddItem(GOItem *newItem);
    void RemoveItem(GOItem *item);
    void Clear();

    void SetSelection(int i);
    void SetSelection(GOItem *item);

    int GetNumItems() const;
    GameObject *GetContainer() const;
    UIScrollPanel *GetScrollPanel() const;

    int GetSelectedIndex() const;
    GOItem* GetSelectedItem() const;

    using SelectionCallback = std::function<void(GOItem *item, Action action)>;
    void SetSelectionCallback(SelectionCallback selectionCallback);

protected:
    UIList();
    virtual ~UIList();

private:
    int m_selectionIndex = -1;
    GOItem *p_itemUnderMouse = nullptr;
    SelectionCallback m_selectionCallback;

    GameObject *p_container = nullptr;
    UIScrollPanel *p_scrollPanel = nullptr;

    static UIList* CreateInto(GameObject *go);
    void Callback(GameObject *item, Action action);

    friend class GameObjectFactory;
    friend class IComponentDriver<UIList>;
};

NAMESPACE_BANG_END

#endif // UILIST_H
