#include "GameObjectInspectorWidgetGroup.h"

#include "Bang/GameObject.h"
#include "Bang/InspectorWidgetFactory.h"

GameObjectInspectorWidgetGroup::GameObjectInspectorWidgetGroup(GameObject *go)
{
    p_gameObject = go;
    for (Component *comp : go->GetComponents())
    {
        InspectorWidget *compIW =
                InspectorWidgetFactory::CreateComponentWidget(comp);
        InsertInspectorWidget(compIW);
    }
}

GameObjectInspectorWidgetGroup::~GameObjectInspectorWidgetGroup()
{

}

String GameObjectInspectorWidgetGroup::GetTitle() const
{
    return p_gameObject->GetName();
}

bool GameObjectInspectorWidgetGroup::NeedsEnableCheckBox() const
{
    return true;
}

void GameObjectInspectorWidgetGroup::OnEnableCheckBoxChanged(bool enabled)
{
    p_gameObject->SetEnabled(enabled);
    OnInspectorWidgetChanged(nullptr);
}

bool GameObjectInspectorWidgetGroup::IsEnabled() const
{
    return p_gameObject->IsEnabled();
}
