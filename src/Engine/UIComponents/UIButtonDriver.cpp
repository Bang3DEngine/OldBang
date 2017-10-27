#include "Bang/UIButtonDriver.h"

#include "Bang/UILabel.h"
#include "Bang/UIFocusTaker.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UITintedButton.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"

USING_NAMESPACE_BANG

UIButtonDriver::UIButtonDriver()
{

}

UIButtonDriver::~UIButtonDriver()
{

}

UIBorderRect *UIButtonDriver::GetBorder() const { return p_border; }
UITextRenderer *UIButtonDriver::GetText() const { return p_text; }
UIImageRenderer *UIButtonDriver::GetBackground() const { return p_background; }
UITintedButton *UIButtonDriver::GetButton() const { return p_button; }

void UIButtonDriver::SetBorder(UIBorderRect *borderRect) { p_border = borderRect; }
void UIButtonDriver::SetText(UITextRenderer *textRenderer) { p_text = textRenderer; }
void UIButtonDriver::SetBackground(UIImageRenderer *imgRenderer)
{
    p_background = imgRenderer;
}
void UIButtonDriver::SetButton(UITintedButton *button)
{
    p_button = button;
}

UIButtonDriver* UIButtonDriver::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);
    REQUIRE_COMPONENT(go, UIFocusTaker);

    UIButtonDriver *buttonDriv = go->AddComponent<UIButtonDriver>();

    UIVerticalLayout *vl = go->AddComponent<UIVerticalLayout>();
    vl->SetPaddingBot(5);
    vl->SetPaddingTop(5);
    vl->SetPaddingRight(5);
    vl->SetPaddingLeft (5);

    UIImageRenderer *bgImg = go->AddComponent<UIImageRenderer>();
    bgImg->SetTint(Color::White);

    UILayoutElement *le = go->AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2(0) );

    UITintedButton *bgWTint = go->AddComponent<UITintedButton>();
    bgWTint->SetMode(UIButtonMode::UseRectTransform);
    bgWTint->AddToTint(go);
    bgWTint->SetIdleTintColor(bgImg->GetTint());
    bgWTint->SetOverTintColor( Color(Vector3(0.95), 1) );
    bgWTint->SetPressedTintColor( Color(Vector3(0.9), 1) );
    bgWTint->AddAgent(go);

    UILabel *label = GameObjectFactory::CreateUILabel();
    label->GetText()->SetLayoutMode(
                UITextRenderer::LayoutMode::SingleLineMinPreferred);
    label->GetText()->SetTextColor(Color::Black);

    buttonDriv->SetBackground(bgImg);
    buttonDriv->SetButton(bgWTint);
    buttonDriv->SetText(label->GetText());

    go->AddChild(label->gameObject);

    return buttonDriv;
}
