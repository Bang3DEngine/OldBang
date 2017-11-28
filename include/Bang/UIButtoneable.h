#ifndef UIBUTTON_H
#define UIBUTTON_H

#include <functional>

#include "Bang/Set.h"
#include "Bang/Input.h"
#include "Bang/Component.h"
#include "Bang/IFocusable.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class GameObject;
FORWARD class IUIButtonListener;

enum class UIButtoneableMode
{
    RectTransform,
    Render
};

class UIButtoneable : public Component,
                      public IFocusable,
                      public IDestroyListener,
                      public EventEmitter<IUIButtonListener>
{
    COMPONENT(UIButtoneable)

public:
    UIButtoneable();
    virtual ~UIButtoneable();

    // Component
    virtual void OnUpdate() override;

    void RegisterButtonPart(GameObject *buttonPart);
    void UnRegisterButtonPart(GameObject *buttonPart);

    using EnterExitCallback = std::function<void(UIButtoneable*)>;
    using ClickedCallback = EnterExitCallback;
    using DoubleClickedCallback = ClickedCallback;
    using DownCallback = std::function<void(UIButtoneable*, MouseButton)>;
    using UpCallback = std::function<void(UIButtoneable*, MouseButton, bool)>;

    void AddMouseEnterCallback(EnterExitCallback callback);
    void AddMouseExitCallback(EnterExitCallback callback);
    void AddMouseDownCallback(DownCallback callback);
    void AddMouseUpCallback(UpCallback callback);
    void AddClickedCallback(ClickedCallback callback);
    void AddDoubleClickedCallback(DoubleClickedCallback callback);

    void SetMode(UIButtoneableMode mode);

    bool IsMouseOverSomePart() const;
    bool IsBeingPressed() const;
    UIButtoneableMode GetMode() const;

    void OnDestroyed(Object *object) override;

private:
    bool m_mouseOver    = false;
    bool m_beingPressed = false;
    UIButtoneableMode m_mode = UIButtoneableMode::RectTransform;

    Set<GameObject*> p_buttonParts;

    Array<EnterExitCallback> m_mouseEnterCallbacks;
    Array<EnterExitCallback> m_mouseExitCallbacks;
    Array<DownCallback> m_mouseDownCallbacks;
    Array<UpCallback> m_mouseUpCallbacks;
    Array<ClickedCallback> m_clickedCallbacks;
    Array<DoubleClickedCallback> m_doubleClickedCallbacks;
};

class IUIButtonListener : public IEventListener
{
protected:
    virtual void OnButton_MouseEnter(UIButtoneable *btn) {}
    virtual void OnButton_MouseExit(UIButtoneable *btn) {}
    virtual void OnButton_MouseDown(UIButtoneable *btn, MouseButton mb) {}
    virtual void OnButton_MouseUp(UIButtoneable *btn, MouseButton mb,
                                  bool insideButton) {}
    virtual void OnButton_Clicked(UIButtoneable *btn) {}
    virtual void OnButton_DoubleClicked(UIButtoneable *btn) {}

    friend class UIButtoneable;
};

NAMESPACE_BANG_END

#endif // UIBUTTON_H
