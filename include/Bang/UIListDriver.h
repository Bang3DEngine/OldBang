#ifndef UILISTDRIVER_H
#define UILISTDRIVER_H

#include <functional>

#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIScrollArea;

class UIListDriver : public IComponentDriver<UIListDriver>,
                     public Component
{
    COMPONENT(UIListDriver);

public:
    enum Action { SelectionIn, SelectionOut, MouseOver, MouseOut,
                  Pressed, DoubleClickedLeft, ClickedLeft, ClickedRight };

    virtual ~UIListDriver();

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
    UIListDriver();

private:
    int m_selectionIndex = 0;
    GameObject *p_mouseOverGo = nullptr;
    SelectionCallback m_selectionCallback;

    UIScrollArea *p_scrollArea = nullptr;

    static UIListDriver* CreateInto(GameObject *go);
    void Callback(GameObject *go, Action action);

    friend class GameObjectFactory;
    friend class IComponentDriver<UIListDriver>;
};

NAMESPACE_BANG_END

#endif // UILISTDRIVER_H
