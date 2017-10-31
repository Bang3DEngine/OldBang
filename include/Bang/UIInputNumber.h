#ifndef UIINPUTNUMBER_H
#define UIINPUTNUMBER_H

#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIInputText;

class UIInputNumber : public IComponentDriver<UIInputNumber>,
                      public Component
{
    COMPONENT(UIInputNumber)

public:
	UIInputNumber();
	virtual ~UIInputNumber();

    void SetNumber(float v);
    float GetNumber() const;

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

