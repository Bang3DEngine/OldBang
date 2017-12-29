#include "Bang/UIComboBox.h"

#include "Bang/UIList.h"
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
    UILabel *labelItem = GameObjectFactory::CreateUILabel();
    labelItem->GetText()->SetContent(label);

    p_list->AddItem(labelItem->GetGameObject());
}

UIComboBox *UIComboBox::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UIComboBox *comboBox = go->AddComponent<UIComboBox>();

    UIList *list = GameObjectFactory::CreateUIList(false);

    list->GetGameObject()->SetParent(go);

    comboBox->p_list = list;

    return comboBox;
}

