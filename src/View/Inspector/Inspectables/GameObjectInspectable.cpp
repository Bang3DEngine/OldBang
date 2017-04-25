#include "GameObjectInspectable.h"

#include "Bang/GameObject.h"

GameObjectInspectable::GameObjectInspectable()
{

}

bool GameObjectInspectable::IsEnabled() const
{
    return GetGameObject()->IsEnabled();
}

bool GameObjectInspectable::NeedsEnabledCheckBox() const
{
    return true;
}

void GameObjectInspectable::OnEnabledChanged(bool enabled)
{
    GetGameObject()->SetEnabled(enabled);
}

String GameObjectInspectable::GetTitleInInspector() const
{
    return GetGameObject()->GetName();
}

InspectorWidget *GameObjectInspectable::GetNewInspectorWidget()
{
    return nullptr;
}

List<IInspectable *> GameObjectInspectable::GetNewInspectablesToShow()
{
    List<IInspectable*> componentsInspectables;
    for (Component *component : GetGameObject()->GetComponents())
    {
        // Inspectable *compInsp = component->
        // componentsInspectables.Add();
    }
    return componentsInspectables;
}

GameObject *GameObjectInspectable::GetGameObject() const
{
    return Object::SCast<GameObject>( GetRelatedSerializableObject() );
}
