#ifndef UIBUTTON_H
#define UIBUTTON_H

#include <functional>

#include "Bang/Set.h"
#include "Bang/Input.h"
#include "Bang/Component.h"

FORWARD class GameObject;
FORWARD class UIButtonListener;

class UIButton : public Component
{
    COMPONENT(UIButton)

public:
    UIButton();
    virtual ~UIButton();

    virtual void OnUpdate() override;

    void AddAgent(GameObject *agent);
    void RemoveAgent(GameObject *agent);

    void AddListener(UIButtonListener *listener);
    void RemoveListener(UIButtonListener *listener);

    using EnterExitCallback = std::function<void(UIButton*)>;
    using ClickedCallback = EnterExitCallback;
    using DownUpCallback = std::function<void(UIButton*, Input::MouseButton)>;

    void AddMouseEnterCallback(EnterExitCallback callback);
    void AddMouseExitCallback(EnterExitCallback callback);
    void AddMouseDownCallback(DownUpCallback callback);
    void AddMouseUpCallback(DownUpCallback callback);
    void AddClickedCallback(ClickedCallback callback);

    bool IsMouseOverSomeAgent() const;
    bool IsBeingPressed() const;

private:
    bool m_mouseOver = false;
    bool m_beingPressed = false;

    Set<GameObject*> p_agents;
    Set<UIButtonListener*> p_listeners;

    Array<EnterExitCallback> m_mouseEnterCallbacks;
    Array<EnterExitCallback> m_mouseExitCallbacks;
    Array<DownUpCallback> m_mouseDownCallbacks;
    Array<DownUpCallback> m_mouseUpCallbacks;
    Array<ClickedCallback> m_clickedCallbacks;
};

class UIButtonListener
{
protected:
    virtual void OnButton_MouseEnter(UIButton *btn) {}
    virtual void OnButton_MouseExit(UIButton *btn) {}
    virtual void OnButton_MouseDown(UIButton *btn, Input::MouseButton mb) {}
    virtual void OnButton_MouseUp(UIButton *btn, Input::MouseButton mb) {}
    virtual void OnButton_Clicked(UIButton *btn) {}

    friend class UIButton;
};

#endif // UIBUTTON_H
