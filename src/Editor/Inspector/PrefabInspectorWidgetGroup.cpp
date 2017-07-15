#include "Bang/PrefabInspectorWidgetGroup.h"

#include "Bang/Prefab.h"
#include "Bang/GameObject.h"

PrefabInspectorWidgetGroup::PrefabInspectorWidgetGroup(Prefab *prefab)
    : GameObjectInspectorWidgetGroup(prefab->InstantiateWithoutStarting())
{
    m_prefabFilepath = prefab->GetFilepath();
}

PrefabInspectorWidgetGroup::~PrefabInspectorWidgetGroup()
{
    delete p_gameObject; // Delete the tmp gameObject created by the prefab
}

void PrefabInspectorWidgetGroup::OnInspectorWidgetChanged(
        IInspectorWidget *inspWidget)
{
    GameObjectInspectorWidgetGroup::OnInspectorWidgetChanged(inspWidget);
    p_gameObject->WriteToFile(m_prefabFilepath);
}
