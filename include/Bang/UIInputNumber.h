#ifndef UIINPUTNUMBER_H
#define UIINPUTNUMBER_H

#include "Bang/UIFocusable.h"
#include "Bang/IFocusListener.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIInputText;

class UIInputNumber : public IComponentDriver<UIInputNumber>,
                      public IFocusListener,
                      public Component
{
    COMPONENT(UIInputNumber)

public:
	UIInputNumber();
	virtual ~UIInputNumber();

    void OnUpdate() override;

    void SetNumber(float v);
    float GetNumber() const;

    // IFocusListener
    virtual void OnFocusTaken() override;
    virtual void OnFocusLost() override;

private:
    float m_value = 0.0f;

    UIInputText *p_inputText = nullptr;

    UIInputText* GetInputText() const;

    // IComponentDriver
    static UIInputNumber *CreateInto(GameObject *go);

    friend class GameObjectFactory;
    friend class IComponentDriver<UIInputText>;
};

NAMESPACE_BANG_END

#endif // UIINPUTNUMBER_H

