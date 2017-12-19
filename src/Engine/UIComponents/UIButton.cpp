#include "Bang/UIButton.h"

#include "Bang/Input.h"
#include "Bang/UIMask.h"
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

void UIButton::OnStart()
{
    Component::OnStart();
    GetButton()->EventEmitter<IFocusListener>::RegisterListener(this);
}

void UIButton::OnUpdate()
{
    Component::OnUpdate();

    if (!IsBlocked())
    {
        if (GetButton()->IsMouseOver())
        {
            if (Input::GetMouseButtonDown(MouseButton::Left))
            {
                GetBackground()->SetTint(Color::DarkGray);
            }
        }

        if (Input::GetMouseButtonUp(MouseButton::Left))
        {
            if (GetButton()->IsMouseOver()) { OnMouseEnter(GetButton()); }
            else { OnMouseExit(GetButton()); }
        }
    }
}

void UIButton::SetBlocked(bool blocked)
{
    m_isBlocked = blocked;

    p_button->IFocusable::SetEmitEvents( !IsBlocked() );
    if (!IsBlocked())
    {
        GetText()->SetTextColor(Color::Black);
        if (GetButton()->IsMouseOver()) { OnMouseEnter(GetButton()); }
        else { OnMouseExit(GetButton()); }
    }
    else
    {
        GetText()->SetTextColor(Color::LightGray);
        GetBackground()->SetTint(Color::DarkGray);
    }
}

void UIButton::SetIconSize(const Vector2i &size)
{
    UILayoutElement *le = GetIcon()->GetGameObject()->
                          GetComponent<UILayoutElement>();
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

bool UIButton::IsBlocked() const { return m_isBlocked; }
UIImageRenderer *UIButton::GetIcon() const { return p_icon; }
UITextRenderer *UIButton::GetText() const { return p_text; }
UIImageRenderer *UIButton::GetBackground() const { return p_background; }
UIFocusable *UIButton::GetButton() const { return p_button; }
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

    UIFocusable *btn = go->AddComponent<UIFocusable>();

    UILabel *label = GameObjectFactory::CreateUILabel();
    label->GetText()->SetTextColor(Color::Black);
    label->GetMask()->SetMasking(false);

    UIImageRenderer *icon = GameObjectFactory::CreateUIImage();
    icon->SetAspectRatioMode(AspectRatioMode::Keep);

    GameObject *iconGo = icon->GetGameObject();
    UILayoutElement *iconLE = iconGo->AddComponent<UILayoutElement>();
    iconLE->SetFlexibleSize(Vector2::Zero);

    UILayoutElement *le = go->AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2(0.0f) );

    buttonDriv->p_icon = icon;
    buttonDriv->p_background = bgImg;
    buttonDriv->p_button = btn;
    buttonDriv->p_text = label->GetText();

    go->SetAsChild(icon->GetGameObject());
    go->SetAsChild(label->GetGameObject());

    buttonDriv->SetIcon(nullptr, Vector2i::Zero, 0);

    return buttonDriv;
}

void UIButton::OnMouseEnter(IFocusable*)
{
    if (!GetButton()->IsBeingPressed())
    {
        GetBackground()->SetTint(Color::LightGray);
    }
}

void UIButton::OnMouseExit(IFocusable*)
{
    if (!GetButton()->IsBeingPressed())
    {
        GetBackground()->SetTint(Color::White);
    }
}
