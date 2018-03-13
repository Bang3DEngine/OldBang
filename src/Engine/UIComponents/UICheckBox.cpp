#include "Bang/UICheckBox.h"

#include "Bang/GameObject.h"
#include "Bang/IconManager.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

Color UICheckBox::IdleColor = Color::White;
Color UICheckBox::OverColor = Color::LightBlue;

UICheckBox::UICheckBox()
{
}

UICheckBox::~UICheckBox()
{
}

void UICheckBox::OnUpdate()
{
    Component::OnUpdate();

    if (GetFocusable()->IsMouseOver())
    {
        GetBackgroundImage()->SetTint( UICheckBox::OverColor );
    }
    else
    {
        GetBackgroundImage()->SetTint( UICheckBox::IdleColor );
    }
}

void UICheckBox::SetChecked(bool checked)
{
    if (checked != IsChecked())
    {
        m_isChecked = checked;

        GetCheckImage()->SetEnabled( IsChecked() );
        EventEmitter<IValueChangedListener>::PropagateToListeners(
                    &IValueChangedListener::OnValueChanged, this);
    }
}

bool UICheckBox::IsChecked() const
{
    return m_isChecked;
}

UIImageRenderer *UICheckBox::GetCheckImage() const
{
    return p_checkImage;
}

UIImageRenderer *UICheckBox::GetBackgroundImage() const
{
    return p_checkBgImage;
}

UILayoutElement *UICheckBox::GetLayoutElement() const
{
    return p_layoutElement;
}

IFocusable *UICheckBox::GetFocusable() const
{
    return p_focusable;
}

UICheckBox *UICheckBox::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    constexpr int CheckBoxSize = 12;

    UICheckBox *checkBox = go->AddComponent<UICheckBox>();

    UIVerticalLayout *vl = go->AddComponent<UIVerticalLayout>();
    vl->SetSpacing(0);
    vl->SetPaddings(0);
    vl->SetChildrenHorizontalStretch(Stretch::None);
    vl->SetChildrenHorizontalAlignment(HorizontalAlignment::Center);

    UILayoutElement *checkBoxLE = go->AddComponent<UILayoutElement>();
    checkBoxLE->SetFlexibleSize( Vector2::Zero );

    GameObject *checkBoxBgImgGo = GameObjectFactory::CreateUIGameObject();
    UIImageRenderer *checkBoxImg = checkBoxBgImgGo->AddComponent<UIImageRenderer>();
    checkBoxImg->SetTint(UICheckBox::IdleColor);

    UIFocusable *focusable = checkBoxBgImgGo->AddComponent<UIFocusable>();
    focusable->AddClickedCallback([checkBox](IFocusable*)
    {
        checkBox->SetChecked( !checkBox->IsChecked() );
    });

    UILayoutElement* checkBoxBgImgLE = checkBoxBgImgGo->AddComponent<UILayoutElement>();
    checkBoxBgImgLE->SetMinSize( Vector2i(CheckBoxSize) );
    checkBoxBgImgLE->SetPreferredSize( checkBoxBgImgLE->GetMinSize() );
    checkBoxBgImgLE->SetFlexibleSize( Vector2::Zero );

    GameObject *checkImgGo = GameObjectFactory::CreateUIGameObject();
    UIImageRenderer *checkImg = checkImgGo->AddComponent<UIImageRenderer>();
    checkImg->SetImageTexture( IconManager::GetCheckIcon().Get() );
    checkImg->SetTint(Color::Black);

    checkBox->p_focusable = focusable;
    checkBox->p_checkImage = checkImg;
    checkBox->p_checkBgImage = checkBoxImg;
    checkBox->p_layoutElement = checkBoxBgImgLE;

    checkBoxBgImgGo->SetParent(go);
    checkImgGo->SetParent(checkBoxBgImgGo);

    checkBox->SetChecked(true);

    return checkBox;
}
