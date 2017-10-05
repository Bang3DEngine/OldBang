#include "Bang/UIButtonDriver.h"

#include "Bang/UILabel.h"
#include "Bang/UIFrameLayout.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UITintedButton.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"

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

UIButtonDriver* UIButtonDriver::CreateInto(GameObject *container)
{
    UIFrameLayout *fl = container->AddComponent<UIFrameLayout>();
    fl->SetPaddings(5);
    fl->SetPaddingTop(5);
    fl->SetPaddingRight(10);
    fl->SetPaddingLeft (10);

    UILayoutElement *le = container->AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2(0) );

    GameObject *bg = GameObjectFactory::CreateUIGameObject(true);
    UIImageRenderer *bgImg = bg->AddComponent<UIImageRenderer>();
    bgImg->SetTint(Color::White);
    bg->SetParent(container);

    GameObject *bgGo = GameObjectFactory::CreateUIGameObject(true);
    bgGo->AddComponent<UIFrameLayout>();

    UILabel *label = GameObjectFactory::CreateUILabel();
    label->GetText()->SetTextColor(Color::Black);

    UITintedButton *bgWTint = bgGo->AddComponent<UITintedButton>();
    bgWTint->SetMode(UIButtonMode::UseRectTransform);
    bgWTint->AddToTint(bg);
    bgWTint->SetIdleTintColor(bgImg->GetTint());
    bgWTint->SetOverTintColor( Color(Vector3(0.95), 1) );
    bgWTint->SetPressedTintColor( Color(Vector3(0.9), 1) );
    bgWTint->AddAgent(bg);

    UIButtonDriver *buttonDriv = container->AddComponent<UIButtonDriver>();
    buttonDriv->SetBackground(bgImg);
    buttonDriv->SetButton(bgWTint);
    buttonDriv->SetText(label->GetText());

    container->AddChild(bgGo);
    bgGo->AddChild(label->gameObject);

    return buttonDriv;
}
