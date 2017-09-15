#ifndef UIGAMEOBJECT_H
#define UIGAMEOBJECT_H

#include "Bang/GameObject.h"
#include "Bang/IFocusListener.h"

NAMESPACE_BANG_BEGIN

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
    UIGameObject(const String& name = "UIGameObject");
    virtual FocusAction OnFocusReceived();

private:
    FocusAction m_defaultFocusAction = FocusAction::PassToParent;
    bool m_hasFocus = false;

    UIGameObject* ReceiveFocus();
    UIGameObject* PropagateFocus(const Vector2 &mouseCoordsNDC);

    // IFocusable
    virtual void OnFocusTaken() override;
    virtual void OnFocusLost() override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    friend class UICanvas;
    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UIGAMEOBJECT_H
