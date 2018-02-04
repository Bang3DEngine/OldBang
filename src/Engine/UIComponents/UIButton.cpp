#include "Bang/UIButton.h"

#include "Bang/Input.h"
#include "Bang/Cursor.h"
#include "Bang/UILabel.h"
#include "Bang/GameObject.h"
#include "Bang/UIRectMask.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

const Color UIButton::IdleColor    = Color::White;
const Color UIButton::OverColor    = Color::VeryLightBlue;
const Color UIButton::BlockedColor = Color::LightGray * 0.8f;
const Color UIButton::PressedColor = Color::DarkGray;

UIButton::UIButton()
{
}

UIButton::~UIButton()
{

}

void UIButton::OnStart()
{
    Component::OnStart();
    GetFocusable()->EventEmitter<IFocusListener>::RegisterListener(this);
}

void UIButton::OnUpdate()
{
    Component::OnUpdate();

    if (!IsBlocked())
    {
        bool over = GetFocusable()->IsMouseOver();
        bool pressed = GetFocusable()->IsBeingPressed();
        if (pressed)
        {
            GetBackground()->SetTint(UIButton::PressedColor);
        }
        else
        {
            if (over) { GetBackground()->SetTint(UIButton::OverColor); }
            else { GetBackground()->SetTint(UIButton::IdleColor); }
        }

        if (over || pressed) { Cursor::Set( Cursor::Type::Hand ); }
    }
    else
    {
        if (GetFocusable()->IsMouseOver()) { Cursor::Set( Cursor::Type::No ); }
        GetBackground()->SetTint(UIButton::BlockedColor);
    }
}

void UIButton::Click(bool doubleClick)
{
    GetFocusable()->Click(doubleClick);
}

void UIButton::SetBlocked(bool blocked)
{
    m_isBlocked = blocked;

    p_button->IFocusable::SetEmitEvents( !IsBlocked() );
    if (!IsBlocked())
    {
        GetText()->SetTextColor(Color::Black);
        if (GetFocusable()->IsMouseOver()) { OnMouseEnter(GetFocusable()); }
        else { OnMouseExit(GetFocusable()); }
    }
    else
    {
        GetText()->SetTextColor(Color::DarkGray);
        GetBackground()->SetTint(UIButton::BlockedColor);
    }
}

void UIButton::SetIconSize(const Vector2i &size)
{
    UILayoutElement *le = GetIcon()->GetGameObject()->
                          GetComponent<UILayoutElement>();
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
UIFocusable *UIButton::GetFocusable() const { return p_button; }
 UILayoutElement *UIButton::GetLayoutElement() const { return p_layoutElement; }
UIDirLayout *UIButton::GetDirLayout() const
{
    return GetGameObject()->GetComponent<UIDirLayout>();
}

UIButton* UIButton::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UIButton *button = go->AddComponent<UIButton>();

    UIHorizontalLayout *hl = go->AddComponent<UIHorizontalLayout>();
    hl->SetPaddingBot(3);
    hl->SetPaddingTop(3);
    hl->SetPaddingRight(3);
    hl->SetPaddingLeft (3);
    hl->SetSpacing(0);

    UILayoutElement *le = go->AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2::Zero );
    le->SetLayoutPriority(1);

    UIImageRenderer *bgImg = go->AddComponent<UIImageRenderer>();
    bgImg->SetTint(Color::White);

    UIFocusable *btn = go->AddComponent<UIFocusable>();

    UILabel *label = GameObjectFactory::CreateUILabel();
    label->GetText()->SetTextColor(Color::Black);
    label->GetMask()->SetMasking(false);
    label->SetFocusEnabled(false);

    UIImageRenderer *icon = GameObjectFactory::CreateUIImage();

    GameObject *iconGo = icon->GetGameObject();
    UILayoutElement *iconLE = iconGo->AddComponent<UILayoutElement>();
    iconLE->SetFlexibleSize(Vector2::Zero);

    button->p_icon = icon;
    button->p_background = bgImg;
    button->p_button = btn;
    button->p_text = label->GetText();
    button->p_layoutElement = le;

    icon->GetGameObject()->SetParent(go);
    label->GetGameObject()->SetParent(go);

    button->SetIcon(nullptr, Vector2i::Zero, 0);

    return button;
}
