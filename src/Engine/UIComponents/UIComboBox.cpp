#include "Bang/UIComboBox.h"

#include <functional>

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
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

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
    GameObject *textGo = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *textRend = textGo->AddComponent<UITextRenderer>();
    textRend->SetContent(label);
    textRend->SetTextSize(11);
    textRend->SetVerticalAlign(VerticalAlignment::Center);
    textRend->SetHorizontalAlign(HorizontalAlignment::Right);

    GetList()->AddItem(textGo);
    m_indexToValue.PushBack( value );
    m_indexToLabel.PushBack( label );

    if (m_selectedIndex < 0) { SetSelectionByIndex(0); }
}

void UIComboBox::SetSelectionByIndex(int index)
{
    ASSERT(index >= 0 && index < int( m_indexToValue.Size() ) );

    if (m_selectedIndex != index)
    {
        m_selectedIndex = index;
        p_currentItemText->SetContent( m_indexToLabel[index] );
        EventEmitter<IValueChangedListener>::PropagateToListeners(
                    &IValueChangedListener::OnValueChanged, this);
    }
}

void UIComboBox::SetSelectionByValue(int value)
{
    int indexOfValue = 0;
    if (m_indexToValue.Contains(value))
    {
        indexOfValue = m_indexToValue.IndexOf(value);
    }
    SetSelectionByIndex(indexOfValue);
}

int UIComboBox::GetSelectedValue() const
{
    if (GetSelectedIndex() >= 0 && GetSelectedIndex() < int( m_indexToValue.Size() ))
    {
        return m_indexToValue[ GetSelectedIndex() ];
    }
    return 0;
}

int UIComboBox::GetSelectedIndex() const
{
    return m_selectedIndex;
}

String UIComboBox::GetSelectedLabel() const
{
    if (GetSelectedIndex() >= 0 && GetSelectedIndex() < int( m_indexToValue.Size() ))
    {
        return m_indexToLabel[ GetSelectedIndex() ];
    }
    return "";
}

UIComboBox *UIComboBox::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UIComboBox *comboBox = go->AddComponent<UIComboBox>();

    UIHorizontalLayout *hl = go->AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalStretch(Stretch::Full);
    hl->SetChildrenVerticalAlignment(VerticalAlignment::Top);
    hl->SetPaddings(3);
    hl->SetSpacing(10);

    UIFocusable *focusable = go->AddComponent<UIFocusable>();
    focusable->AddClickedCallback([comboBox](IFocusable*)
    {
        comboBox->GetList()->GetGameObject()->SetEnabled(true);
    });

    GameObject *currentItemTextGo = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *currentItemText = currentItemTextGo->AddComponent<UITextRenderer>();
    currentItemText->SetContent("Current");
    currentItemText->SetTextSize(11);
    currentItemText->SetHorizontalAlign(HorizontalAlignment::Right);

    UIImageRenderer *bg = go->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::White);

    UIImageRenderer *downArrowIcon = GameObjectFactory::CreateUIImage();
    downArrowIcon->SetImageTexture( IconManager::GetDownArrowIcon().Get() );
    downArrowIcon->SetTint( Color::Black );

    GameObject *downArrowIconGo = downArrowIcon->GetGameObject();
    UILayoutElement *downArrowLE = downArrowIconGo->AddComponent<UILayoutElement>();
    downArrowLE->SetMinSize( Vector2i(6) );

    UIList *list = GameObjectFactory::CreateUIList(false);
    GameObject *listGo = list->GetGameObject();
    list->SetWideSelectionMode(true);
    list->SetSelectionCallback([comboBox](GameObject *item, UIList::Action action)
    {
        comboBox->OnListSelectionCallback(item, action);
    });
    listGo->SetEnabled(false);

    UIImageRenderer *listBG = listGo->AddComponent<UIImageRenderer>();
    listBG->SetTint(Color::White);

    listGo->AddComponent<UILayoutIgnorer>();
    RectTransform *contRT = listGo->GetRectTransform();
    contRT->SetAnchors( Vector2(1, -1) );
    contRT->SetPivotPosition( Vector2(1, 1) );
    contRT->TranslateLocal(Vector3(0.0f, 0.0f, -0.0001f));

    UIContentSizeFitter *csf = listGo->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);

    currentItemTextGo->SetParent(go);
    downArrowIconGo->SetParent(go);
    listGo->SetParent(go);

    comboBox->p_currentItemText = currentItemText;
    comboBox->p_list = list;

    return comboBox;
}

void UIComboBox::OnListSelectionCallback(GameObject *item, UIList::Action action)
{
    int indexOfItem = p_list->GetItems().IndexOf(item);
    ASSERT(indexOfItem >= 0);

    switch (action)
    {
        case UIList::Action::ClickedLeft:
        case UIList::Action::ClickedRight:
        case UIList::Action::Pressed:
            SetSelectionByIndex(indexOfItem);
            GetList()->GetGameObject()->SetEnabled(false);
            break;

        default: break;
    }
}

UIList *UIComboBox::GetList() const
{
    return p_list;
}
