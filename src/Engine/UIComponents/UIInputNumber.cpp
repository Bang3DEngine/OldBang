#include "Bang/UIInputNumber.h"

#include "Bang/UIInputText.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

UIInputNumber::UIInputNumber()
{
}

UIInputNumber::~UIInputNumber()
{
}

void UIInputNumber::OnUpdate()
{
    Component::OnUpdate();
}

void UIInputNumber::SetNumber(float v)
{
    m_value = v;
    String vStr = String::ToString(v, 2);
    GetInputText()->GetText()->SetContent(vStr);
}

float UIInputNumber::GetNumber() const { return m_value; }

void UIInputNumber::OnFocusTaken()
{
    IFocusListener::OnFocusTaken();
}

void UIInputNumber::OnFocusLost()
{
    IFocusListener::OnFocusLost();
    std::istringstream iss( GetInputText()->GetText()->GetContent() );
    float number = 0.0f;
    iss >> number;
    SetNumber(number);
}

UIInputText *UIInputNumber::GetInputText() const { return p_inputText; }

UIInputNumber *UIInputNumber::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    GameObjectFactory::CreateUIInputTextInto(go);

    UIInputNumber *inputNumber = go->AddComponent<UIInputNumber>();

    UIInputText *inputText = go->GetComponent<UIInputText>();
    inputText->SetAllowedCharacters("0123456789.,-+");
    inputText->GetText()->SetHorizontalAlign(HorizontalAlignment::Right);
    inputText->GetText()->SetTextSize(10);

    inputNumber->p_inputText = inputText;

    inputNumber->SetNumber(0.0f);

    return inputNumber;
}

