#ifndef UIGAMEOBJECT_H
#define UIGAMEOBJECT_H

#include "Bang/GameObject.h"
#include "Bang/IFocusListener.h"

FORWARD class RectTransform;

#define UIGAMEOBJECT(ClassName) GAMEOBJECT(ClassName)

class UIGameObject : public GameObject,
                     public IFocusListener
{
    UIGAMEOBJECT(UIGameObject)

public:
    UIGameObject(const String& name = "UIGameObject");

    RectTransform *const& rectTransform = m_rectTransform;

    bool IsMouseOver() const;
    bool HasFocus() const;

    void SetDefaultFocusAction(FocusAction focusAction);

protected:
    virtual FocusAction OnFocusReceived();

private:
    FocusAction m_defaultFocusAction = FocusAction::PassToParent;
    RectTransform *m_rectTransform = nullptr;
    bool m_hasFocus = false;

    UIGameObject* ReceiveFocus();
    UIGameObject* PropagateFocus(const Vector2 &mouseCoordsNDC);

    // IFocusable
    virtual void OnFocusTaken() override;
    virtual void OnFocusLost() override;

    friend class UICanvas;
};

#endif // UIGAMEOBJECT_H
