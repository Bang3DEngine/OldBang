#include "Bang/GUIButton.h"

#include "Bang/GUILabel.h"
#include "Bang/GUIImage.h"

GUIButton::GUIButton(const String &labelText) : UIGameObject("GUIButton")
{
    p_bgImage = new GUIImage();
    p_bgImage->GetImage()->SetTint(Color::White);
    p_bgImage->SetParent(this);

    p_label = new GUILabel(labelText);
    p_label->GetText()->SetTint(Color::Black);
    p_label->SetParent(this);

    p_labelTinter = AddComponent<UIButtonTinter>();
    p_labelTinter->AddAgent(this);
    p_labelTinter->AddGameObjectToTint(p_label);
    p_labelTinter->SetIdleTintColor(p_label->GetText()->GetTint());
    p_labelTinter->SetOverTintColor(Color::Black);
    p_labelTinter->SetPressedTintColor(Color::White);

    p_bgTinter = AddComponent<UIButtonTinter>();
    p_bgTinter->AddGameObjectToTint(p_bgImage);
    p_bgTinter->SetIdleTintColor(p_bgImage->GetImage()->GetTint());
    p_bgTinter->SetOverTintColor(Color::Gray);
    p_bgTinter->SetPressedTintColor(Color::Black);
    p_bgTinter->AddAgent(this);
}

GUIButton::~GUIButton()
{
}

GUILabel *GUIButton::GetLabel() const { return p_label; }
GUIImage *GUIButton::GetImage() const { return p_bgImage; }
UIButtonTinter *GUIButton::GetLabelTinter() const { return p_labelTinter; }
UIButtonTinter *GUIButton::GetBackgroundTinter() const { return p_bgTinter; }
