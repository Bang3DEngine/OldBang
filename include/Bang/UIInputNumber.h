#ifndef UIINPUTNUMBER_H
#define UIINPUTNUMBER_H

#include "Bang/UIFocusable.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IFocusListener.h"
#include "Bang/IComponentDriver.h"
#include "Bang/IValueChangedListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIInputText;

class UIInputNumber : public IComponentDriver<UIInputNumber>,
                      public EventEmitter<IValueChangedListener>,
                      public IValueChangedListener,
                      public IFocusListener,
                      public Component
{
    COMPONENT(UIInputNumber)

public:
	UIInputNumber();
	virtual ~UIInputNumber();

    void OnStart() override;
    void OnUpdate() override;

    void SetNumber(float v);
    float GetNumber() const;

    // IFocusListener
    virtual void OnFocusTaken() override;
    virtual void OnFocusLost() override;

private:
    float m_value = 0.0f;
    UIInputText *p_inputText = nullptr;

    void UpdateValueFromText();
    UIInputText* GetInputText() const;

    // IValueChangedListener
    void OnValueChanged(Object*) override;

    // IComponentDriver
    static UIInputNumber *CreateInto(GameObject *go);

    friend class GameObjectFactory;
    friend class IComponentDriver<UIInputText>;
};

NAMESPACE_BANG_END

#endif // UIINPUTNUMBER_H

