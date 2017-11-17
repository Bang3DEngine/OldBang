#include "Bang/UIButton.h"

#include "Bang/UILabel.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

UIButton::UIButton()
{

}

UIButton::~UIButton()
{

}

void UIButton::SetIconSize(const Vector2i &size)
{
    UILayoutElement *le = GetIcon()->GetGameObject()->GetComponent<UILayoutElement>();
    le->SetMinSize(size);
    le->SetPreferredSize(size);
}

void UIButton::SetIconTexture(Texture2D *texture)
{
    GetIcon()->SetImageTexture(texture);
}

void UIButton::SetIconSpacingWithText(int spacingWithText)
{
    GetGameObject()->GetComponent<UIDirLayout>()->SetSpacing(spacingWithText);
}

void UIButton::SetIcon(Texture2D *texture, const Vector2i &size,
                             int spacingWithText)
{
    SetIconTexture(texture);
    SetIconSize(size);
    SetIconSpacingWithText(spacingWithText);
}

UIImageRenderer *UIButton::GetIcon() const { return p_icon; }
UITextRenderer *UIButton::GetText() const { return p_text; }
UIImageRenderer *UIButton::GetBackground() const { return p_background; }
UIButtoneable *UIButton::GetButton() const { return p_button; }
UIDirLayout *UIButton::GetDirLayout() const
{
    return GetGameObject()->GetComponent<UIDirLayout>();
}

UIButton* UIButton::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UIButton *buttonDriv = go->AddComponent<UIButton>();

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

    UIButtoneable *btn = go->AddComponent<UIButtoneable>();
    btn->SetMode(UIButtoneableMode::RectTransform);
    btn->RegisterButtonPart(go);
    btn->EventEmitter<IUIButtonListener>::RegisterListener(buttonDriv);

    UILabel *label = GameObjectFactory::CreateUILabel();
    label->GetText()->SetTextColor(Color::Black);

    UIImageRenderer *icon = GameObjectFactory::CreateUIImage();
    icon->SetAspectRatioMode(AspectRatioMode::Keep);

    GameObject *iconGo = icon->GetGameObject();
    UILayoutElement *iconLE = iconGo->AddComponent<UILayoutElement>();
    iconLE->SetFlexibleSize(Vector2::Zero);

    buttonDriv->p_icon = icon;
    buttonDriv->p_background = bgImg;
    buttonDriv->p_button = btn;
    buttonDriv->p_text = label->GetText();

    go->SetAsChild(icon->GetGameObject());
    go->SetAsChild(label->GetGameObject());

    buttonDriv->SetIcon(nullptr, Vector2i::Zero, 0);

    return buttonDriv;
}

void UIButton::OnButton_MouseEnter(UIButtoneable *btn)
{
    if (!GetButton()->IsBeingPressed())
    {
        GetBackground()->SetTint(Color::LightGray);
    }
}

void UIButton::OnButton_MouseExit(UIButtoneable *btn)
{
    if (!GetButton()->IsBeingPressed())
    {
        GetBackground()->SetTint(Color::White);
    }
}

void UIButton::OnButton_MouseDown(UIButtoneable *btn, MouseButton mb)
{
    if (mb == MouseButton::Left)
    {
        GetBackground()->SetTint(Color::Black);
    }
}

void UIButton::OnButton_MouseUp(UIButtoneable *btn, MouseButton mb, bool inside)
{
    if (mb == MouseButton::Left)
    {
        if (inside) { OnButton_MouseEnter(btn); }
        else { OnButton_MouseExit(btn); }
    }
}
