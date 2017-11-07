#include "Bang/UIButtonDriver.h"

#include "Bang/UILabel.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UITintedButton.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

UIButtonDriver::UIButtonDriver()
{

}

UIButtonDriver::~UIButtonDriver()
{

}

void UIButtonDriver::SetIcon(Texture2D *texture, const Vector2i &size,
                             int spacingWithText)
{
    GetIcon()->SetImageTexture(texture);

    UILayoutElement *le = GetIcon()->GetGameObject()->GetComponent<UILayoutElement>();
    le->SetMinSize(size);

    GetGameObject()->GetComponent<UIDirLayout>()->SetSpacing(spacingWithText);
}

UIImageRenderer *UIButtonDriver::GetIcon() const { return p_icon; }
UITextRenderer *UIButtonDriver::GetText() const { return p_text; }
UIImageRenderer *UIButtonDriver::GetBackground() const { return p_background; }
UITintedButton *UIButtonDriver::GetButton() const { return p_button; }
UIDirLayout *UIButtonDriver::GetDirLayout() const
{
    return GetGameObject()->GetComponent<UIDirLayout>();
}

UIButtonDriver* UIButtonDriver::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UIButtonDriver *buttonDriv = go->AddComponent<UIButtonDriver>();

    UIHorizontalLayout *hl = go->AddComponent<UIHorizontalLayout>();
    hl->SetPaddingBot(3);
    hl->SetPaddingTop(3);
    hl->SetPaddingRight(3);
    hl->SetPaddingLeft (3);
    hl->SetSpacing(5);

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
    bgWTint->RegisterEmitter(go);

    UILabel *label = GameObjectFactory::CreateUILabel();
    label->GetText()->SetTextColor(Color::Black);

    UIImageRenderer *icon = GameObjectFactory::CreateUIImage();
    icon->SetAspectRatioMode(AspectRatioMode::Keep);

    GameObject *iconGo = icon->GetGameObject();
    UILayoutElement *iconLE = iconGo->AddComponent<UILayoutElement>();
    iconLE->SetFlexibleSize(Vector2::Zero);

    buttonDriv->p_icon = icon;
    buttonDriv->p_background = bgImg;
    buttonDriv->p_button = bgWTint;
    buttonDriv->p_text = label->GetText();

    go->AddChild(icon->GetGameObject());
    go->AddChild(label->GetGameObject());

    buttonDriv->SetIcon(nullptr, Vector2i::Zero, 0);

    return buttonDriv;
}
