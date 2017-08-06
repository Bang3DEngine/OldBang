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
public:
    UIButton();
    virtual ~UIButton();

    virtual void OnUpdate() override;

    void AddAgent(GameObject *agent);
    void RemoveAgent(GameObject *agent);

    void AddListener(UIButtonListener *listener);
    void RemoveListener(UIButtonListener *listener);

    using EnterExitCallback = std::function<void(UIButton*)>;
    using DownUpCallback = std::function<void(UIButton*, Input::MouseButton)>;

    void AddMouseEnterCallback(EnterExitCallback callback);
    void AddMouseExitCallback(EnterExitCallback callback);
    void AddMouseDownCallback(DownUpCallback callback);
    void AddMouseUpCallback(DownUpCallback callback);

private:
    bool m_mouseOver = false;

    Set<GameObject*> p_agents;
    Set<UIButtonListener*> p_listeners;

    Array<EnterExitCallback> m_mouseEnterCallbacks;
    Array<EnterExitCallback> m_mouseExitCallbacks;
    Array<DownUpCallback> m_mouseDownCallbacks;
    Array<DownUpCallback> m_mouseUpCallbacks;
};

class UIButtonListener
{
protected:
    virtual void OnButton_MouseEnter(UIButton *btn) {}
    virtual void OnButton_MouseExit(UIButton *btn) {}
    virtual void OnButton_MouseDown(UIButton *btn,
                                    Input::MouseButton mouseButton) {}
    virtual void OnButton_MouseUp(UIButton *btn,
                                  Input::MouseButton mouseButton) {}

    friend class UIButton;
};

#endif // UIBUTTON_H
