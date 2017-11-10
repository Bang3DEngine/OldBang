#ifndef UILIST_H
#define UILIST_H

#include <functional>

#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIScrollArea;

class UIList : public IComponentDriver<UIList>,
               public Component
{
    COMPONENT(UIList);

public:
    enum Action { SelectionIn, SelectionOut, MouseOver, MouseOut,
                  Pressed, DoubleClickedLeft, ClickedLeft, ClickedRight };

    virtual ~UIList();

    void OnUpdate() override;
    void AddElement(GameObject *go);
    void RemoveElement(GameObject *go);
    void Clear();

    void SetSelection(int i);
    void SetSelection(GameObject *go);

    GameObject *GetContainer() const;

    int GetSelectedIndex() const;
    GameObject* GetSelectedGameObject() const;

    using SelectionCallback = std::function<void(GameObject *go, Action action)>;
    void SetSelectionCallback(SelectionCallback selectionCallback);

protected:
    UIList();

private:
    int m_selectionIndex = 0;
    GameObject *p_mouseOverGo = nullptr;
    SelectionCallback m_selectionCallback;

    GameObject *p_container = nullptr;
    UIScrollPanel *p_scrollPanel = nullptr;

    UIScrollPanel *GetScrollPanel() const;

    static UIList* CreateInto(GameObject *go);
    void Callback(GameObject *go, Action action);

    friend class GameObjectFactory;
    friend class IComponentDriver<UIList>;
};

NAMESPACE_BANG_END

#endif // UILIST_H
