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
}

PrefabAssetFileInspectable::~PrefabAssetFileInspectable()
{
    Inspector::GetInstance()->Clear();
    if (m_prefabTempGameObject) { delete m_prefabTempGameObject; }
}

void PrefabAssetFileInspectable::ShowInInspector()
{
    //Inspector::GetInstance()->ShowInspectable(m_prefabTempGameObject);
}

void PrefabAssetFileInspectable::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);
    m_prefabTempGameObject->Read(xmlInfo);
}

void PrefabAssetFileInspectable::OnInspectorSlotChanged(
                                             InspectorWidget *inspectorWidget)
{
    AssetsManager::UpdateAsset(m_file.GetAbsolutePath(),
                               m_prefabTempGameObject->GetXMLInfo());
}

void PrefabAssetFileInspectable::Write(XMLNode *xmlInfo) const
{
    m_prefabTempGameObject->Write(xmlInfo);
}

GameObject *PrefabAssetFileInspectable::GetPrefabTempGameObject() const
{
    return m_prefabTempGameObject;
}



