#ifndef UIINTERACTIVE_H
#define UIINTERACTIVE_H

#include <functional>

#include "Bang/Set.h"
#include "Bang/Input.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class GameObject;
FORWARD class UIInteractiveListener;

enum UIInteractiveMode
{
    UseRectTransform,
    UseRender
};

class UIInteractive : public Component
{
    COMPONENT(UIInteractive)

public:
    UIInteractive();
    virtual ~UIInteractive();

    // Component
    virtual void OnUpdate() override;

    void AddAgent(GameObject *agent);
    void RemoveAgent(GameObject *agent);

    void AddListener(UIInteractiveListener *listener);
    void RemoveListener(UIInteractiveListener *listener);

    using EnterExitCallback = ::std::function<void(UIInteractive*)>;
    using ClickedCallback = EnterExitCallback;
    using DownUpCallback = ::std::function<void(UIInteractive*, Input::MouseButton)>;

    void AddMouseEnterCallback(EnterExitCallback callback);
    void AddMouseExitCallback(EnterExitCallback callback);
    void AddMouseDownCallback(DownUpCallback callback);
    void AddMouseUpCallback(DownUpCallback callback);
    void AddClickedCallback(ClickedCallback callback);

    void SetMode(UIInteractiveMode mode);

    bool IsMouseOverSomeAgent() const;
    bool IsBeingPressed() const;
    UIInteractiveMode GetMode() const;

private:
    bool m_mouseOver    = false;
    bool m_beingPressed = false;
    UIInteractiveMode m_mode = UIInteractiveMode::UseRender;

    Set<GameObject*> p_agents;
    Set<UIInteractiveListener*> p_listeners;

    Array<EnterExitCallback> m_mouseEnterCallbacks;
    Array<EnterExitCallback> m_mouseExitCallbacks;
    Array<DownUpCallback> m_mouseDownCallbacks;
    Array<DownUpCallback> m_mouseUpCallbacks;
    Array<ClickedCallback> m_clickedCallbacks;
};

class UIInteractiveListener
{
protected:
    virtual void OnButton_MouseEnter(UIInteractive *btn) {}
    virtual void OnButton_MouseExit(UIInteractive *btn) {}
    virtual void OnButton_MouseDown(UIInteractive *btn, Input::MouseButton mb) {}
    virtual void OnButton_MouseUp(UIInteractive *btn, Input::MouseButton mb) {}
    virtual void OnButton_Clicked(UIInteractive *btn) {}

    friend class UIInteractive;
};

NAMESPACE_BANG_END

#endif // UIINTERACTIVE_H
