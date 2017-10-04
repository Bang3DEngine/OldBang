#ifndef UIFOCUSTAKER_H
#define UIFOCUSTAKER_H

#include "Bang/Component.h"
#include "Bang/IFocusListener.h"

NAMESPACE_BANG_BEGIN

class UIFocusTaker : public Component,
                     public IFocusListener
{
    COMPONENT(UIFocusTaker);

public:
    UIFocusTaker();
    virtual ~UIFocusTaker();

    void SetDefaultFocusAction(FocusAction focusAction);
    bool IsMouseOver() const;
    bool HasFocus() const;

    UIFocusTaker *ReceiveFocus();
    UIFocusTaker *PropagateFocus(const Vector2 &mouseCoordsNDC);
    virtual FocusAction OnFocusReceived();

    // IFocusListener
    void OnFocusLost() override;
    void OnFocusTaken() override;

    // SerializableObject
    void ExportXML(XMLNode *xmlInfo) const;
    void ImportXML(const XMLNode &xmlInfo);


private:
    FocusAction m_defaultFocusAction = FocusAction::PassToParent;
    bool m_hasFocus = false;

    friend class UICanvas;
};

NAMESPACE_BANG_END

#endif // UIFOCUSTAKER_H
