#ifndef UIBUTTON_H
#define UIBUTTON_H

#include <functional>

#include "Bang/Set.h"
#include "Bang/Input.h"
#include "Bang/Component.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class GameObject;
FORWARD class IUIButtonListener;

enum UIButtonMode
{
    UseRectTransform,
    UseRender
};

class UIButton : public Component,
                 public IDestroyListener,
                 public EventEmitter<IUIButtonListener>
{
    COMPONENT(UIButton)

public:
    UIButton();
    virtual ~UIButton();

    // Component
    virtual void OnUpdate() override;

    void RegisterButtonPart(GameObject *emitter);
    void UnRegisterButtonPart(GameObject *emitter);

    using EnterExitCallback = std::function<void(UIButton*)>;
    using ClickedCallback = EnterExitCallback;
    using DoubleClickedCallback = ClickedCallback;
    using DownUpCallback = std::function<void(UIButton*, MouseButton)>;

    void AddMouseEnterCallback(EnterExitCallback callback);
    void AddMouseExitCallback(EnterExitCallback callback);
    void AddMouseDownCallback(DownUpCallback callback);
    void AddMouseUpCallback(DownUpCallback callback);
    void AddClickedCallback(ClickedCallback callback);
    void AddDoubleClickedCallback(DoubleClickedCallback callback);

    void SetMode(UIButtonMode mode);

    bool IsMouseOverSomePart() const;
    bool IsBeingPressed() const;
    UIButtonMode GetMode() const;

    void OnBeforeDestroyed(Object *object) override;

private:
    bool m_mouseOver    = false;
    bool m_beingPressed = false;
    UIButtonMode m_mode = UIButtonMode::UseRectTransform;

    Set<GameObject*> p_buttonParts;

    Array<EnterExitCallback> m_mouseEnterCallbacks;
    Array<EnterExitCallback> m_mouseExitCallbacks;
    Array<DownUpCallback> m_mouseDownCallbacks;
    Array<DownUpCallback> m_mouseUpCallbacks;
    Array<ClickedCallback> m_clickedCallbacks;
    Array<DoubleClickedCallback> m_doubleClickedCallbacks;
};

class IUIButtonListener : public IEventListener
{
protected:
    virtual void OnButton_MouseEnter(UIButton *btn) {}
    virtual void OnButton_MouseExit(UIButton *btn) {}
    virtual void OnButton_MouseDown(UIButton *btn, MouseButton mb) {}
    virtual void OnButton_MouseUp(UIButton *btn, MouseButton mb) {}
    virtual void OnButton_Clicked(UIButton *btn) {}
    virtual void OnButton_DoubleClicked(UIButton *btn) {}

    friend class UIButton;
};

NAMESPACE_BANG_END

#endif // UIBUTTON_H
