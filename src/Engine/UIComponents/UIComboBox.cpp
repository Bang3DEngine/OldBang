#include "Bang/UIComboBox.h"

#include "Bang/UILabel.h"
#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/IconManager.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"


USING_NAMESPACE_BANG

UIComboBox::UIComboBox()
{
}

UIComboBox::~UIComboBox()
{
}

void UIComboBox::OnUpdate()
{
    Component::OnUpdate();
}

void UIComboBox::AddItem(const String &label, int value)
{

}

UIComboBox *UIComboBox::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    constexpr int CheckBoxSize = 12;

    UIComboBox *comboBox = go->AddComponent<UIComboBox>();

    UIVerticalLayout *vl = go->AddComponent<UIVerticalLayout>();
    vl->SetSpacing(0);
    vl->SetPaddings(0);
    vl->SetChildrenHorizontalStretch(Stretch::None);
    vl->SetChildrenHorizontalAlignment(HorizontalAlignment::Center);

    UILayoutElement *checkBoxLE = go->AddComponent<UILayoutElement>();
    checkBoxLE->SetFlexibleSize( Vector2::Zero );

    GameObject *checkBoxBgImgGo = GameObjectFactory::CreateUIGameObject();
    UIImageRenderer *checkBoxImg = checkBoxBgImgGo->AddComponent<UIImageRenderer>();
    checkBoxImg->GetMaterial()->SetDiffuseColor(Color::White);

    UIFocusable *focusable = checkBoxBgImgGo->AddComponent<UIFocusable>();
    focusable->AddClickedCallback([comboBox](IFocusable*)
    {
    });

    UILayoutElement* checkBoxBgImgLE = checkBoxBgImgGo->AddComponent<UILayoutElement>();
    checkBoxBgImgLE->SetMinSize( Vector2i(CheckBoxSize) );
    checkBoxBgImgLE->SetPreferredSize( checkBoxBgImgLE->GetMinSize() );
    checkBoxBgImgLE->SetFlexibleSize( Vector2::Zero );

    GameObject *checkImgGo = GameObjectFactory::CreateUIGameObject();
    UIImageRenderer *checkImg = checkImgGo->AddComponent<UIImageRenderer>();
    checkImg->SetImageTexture( IconManager::GetCheckIcon().Get() );
    checkImg->GetMaterial()->SetDiffuseColor(Color::Black);

    checkBoxBgImgGo->SetParent(go);
    checkImgGo->SetParent(checkBoxBgImgGo);

    return comboBox;
}

