#include "Bang/GUIButton.h"

#include "Bang/GUILabel.h"
#include "Bang/GUIImage.h"

GUIButton::GUIButton(const String &labelText) : UIGameObject("GUIButton")
{
    p_bgImage = new GUIImage();
    p_bgImage->GetImage()->SetTint(Color::White);
    p_bgImage->SetParent(this);

    p_label = new GUILabel(labelText);
    p_label->GetText()->SetTextColor(Color::Black);
    p_label->SetParent(this);

    // Create aux UIGo to be able to render border rect after bg
    UIGameObject *aux = new UIGameObject("GUIButton_Aux");
    p_borderRect = aux->AddComponent<UIBorderRect>();
    p_borderRect->SetLineColor(Color::Purple);
    aux->SetParent(this);

    p_labelTinter = AddComponent<UIButtonTinter>();
    p_labelTinter->AddAgent(this);
    p_labelTinter->AddGameObjectToTint( GetLabel() );
    p_labelTinter->SetIdleTintColor(GetLabel()->GetText()->GetTextColor());
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

void GUIButton::AddClickedCallback(UIButton::ClickedCallback callback)
{
    p_bgTinter->AddClickedCallback(callback);
}
