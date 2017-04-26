#include "Bang/PrefabAssetFileInspectable.h"

#include "Bang/Debug.h"
#include "Bang/Prefab.h"
#include "Bang/Inspector.h"
#include "Bang/GameObject.h"
#include "Bang/EditorWindow.h"
#include "Bang/AssetsManager.h"
#include "Bang/InspectorWidget.h"
#include "Bang/FileReferencesManager.h"


PrefabAssetFileInspectable::PrefabAssetFileInspectable()
{
}

PrefabAssetFileInspectable::PrefabAssetFileInspectable(const File &f) :
    m_file(f)
{
    Prefab prefab;
    prefab.ReadFromFile(f.GetAbsolutePath());
    m_prefabTempGameObject = prefab.InstantiateWithoutStarting();

    SetRelatedSerializableObject(m_prefabTempGameObject);
}

PrefabAssetFileInspectable::~PrefabAssetFileInspectable()
{
    if (m_prefabTempGameObject) { delete m_prefabTempGameObject; }
}

void PrefabAssetFileInspectable::CloneInto(ICloneable *clone) const
{
    Inspectable<GameObject>::CloneInto(clone);
    PrefabAssetFileInspectable *pfinsp =
                        Object::SCast<PrefabAssetFileInspectable>(clone);
    pfinsp->m_file = m_file;
    pfinsp->m_prefabTempGameObject = new GameObject();
    m_prefabTempGameObject->CloneInto(pfinsp->m_prefabTempGameObject);
}

void PrefabAssetFileInspectable::OnInspectorSlotChanged(
                                             InspectorWidget *inspectorWidget)
{
    AssetsManager::UpdateAsset(m_file.GetAbsolutePath(),
                               m_prefabTempGameObject->GetXMLInfo());
}

GameObject *PrefabAssetFileInspectable::GetPrefabTempGameObject() const
{
    return m_prefabTempGameObject;
}



