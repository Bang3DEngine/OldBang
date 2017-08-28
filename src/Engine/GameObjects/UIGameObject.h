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
    bool IsMouseOver() const;
    bool HasFocus() const;

    RectTransform* GetRectTransform() const;

    void SetDefaultFocusAction(FocusAction focusAction);

protected:
    virtual FocusAction OnFocusReceived();

private:
    FocusAction m_defaultFocusAction = FocusAction::PassToParent;
    bool m_hasFocus = false;

    UIGameObject* ReceiveFocus();
    UIGameObject* PropagateFocus(const Vector2 &mouseCoordsNDC);

    UIGameObject(const String& name = "UIGameObject");

    // IFocusable
    virtual void OnFocusTaken() override;
    virtual void OnFocusLost() override;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    friend class UICanvas;
    friend class GameObjectFactory;
};

#endif // UIGAMEOBJECT_H
