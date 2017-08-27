#include "Bang/GUIButton.h"

#include "Bang/GUILabel.h"
#include "Bang/UIImageRenderer.h"

GUIButton::GUIButton()
{
}

GUIButton::~GUIButton()
{
}

GUILabel *GUIButton::GetLabel() const { return p_label; }
UIImageRenderer *GUIButton::GetImage() const { return p_bgImage; }

void GUIButton::AddClickedCallback(UIButton::ClickedCallback callback)
{
    p_bgTinter->AddClickedCallback(callback);
}

UIGameObject *GUIButton::CreateGameObject()
{
    UIGameObject *go = new UIGameObject("GUIButton");

    UIGameObject *bg = new UIGameObject("GUIButton_Background");
    bg->AddComponent<UIImageRenderer>();
    bg->SetParent(go);

    GUILabel *label = new GUILabel();
    label->SetName("GUIButton_Label");
    label->SetParent(go);

    GUIButton *button = go->AddComponent<GUIButton>();
    button->InitGameObject();

    return go;
}

void GUIButton::RetrieveReferences()
{
    UIGameObject *go = GetGameObject(); ENSURE(go);
    p_bgImage = go->FindInChildren("GUIButton_Background")->GetComponent<UIImageRenderer>();
    p_label = SCAST<GUILabel*>(go->FindInChildren("GUIButton_Label"));
    p_borderRect = go->GetComponent<UIBorderRect>();
    p_labelTinter = go->GetComponent<UIButtonTinter>();
    p_bgTinter = go->GetComponent<UIButtonTinter>();
}

void GUIButton::InitGameObject()
{
    RetrieveReferences();

    p_bgImage->SetTint(Color::White);

    p_label->GetText()->SetTextColor(Color::Black);

    p_borderRect = GetGameObject()->AddComponent<UIBorderRect>();
    p_borderRect->SetLineColor(Color::Purple);

    p_labelTinter = GetGameObject()->AddComponent<UIButtonTinter>();
    p_labelTinter->AddAgent(GetGameObject());
    p_labelTinter->AddGameObjectToTint( GetLabel() );
    p_labelTinter->SetIdleTintColor(GetLabel()->GetText()->GetTextColor());
    p_labelTinter->SetOverTintColor(Color::Black);
    p_labelTinter->SetPressedTintColor(Color::White);

    p_bgTinter = GetGameObject()->AddComponent<UIButtonTinter>();
    p_bgTinter->AddGameObjectToTint(p_bgImage->GetGameObject());
    p_bgTinter->SetIdleTintColor(p_bgImage->GetTint());
    p_bgTinter->SetOverTintColor(Color::Gray);
    p_bgTinter->SetPressedTintColor(Color::Black);
    p_bgTinter->AddAgent(GetGameObject());
}
