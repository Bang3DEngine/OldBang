#include "Bang/PrefabAssetFileInspectable.h"

#include "Bang/Debug.h"
#include "Bang/Prefab.h"
#include "Bang/Inspector.h"
#include "Bang/GameObject.h"
#include "Bang/EditorWindow.h"
#include "Bang/AssetsManager.h"
#include "Bang/InspectorWidget.h"
#include "Bang/FileReferencesManager.h"

PrefabAssetFileInspectable::PrefabAssetFileInspectable(const File &f) :
    m_file(f)
{
    Prefab *prefab = new Prefab();
    prefab->ReadFromFile(f.GetAbsolutePath());
    m_prefabTempGameObject = prefab->InstantiateWithoutStarting();
    delete prefab;
}

PrefabAssetFileInspectable::~PrefabAssetFileInspectable()
{
    Inspector::GetInstance()->Clear();
    if (m_prefabTempGameObject)
    {
        delete m_prefabTempGameObject;
    }
}

void PrefabAssetFileInspectable::ShowInInspector()
{
    Inspector::GetInstance()->ShowPrefabInspectableInfo(this);
}

void PrefabAssetFileInspectable::OnInspectorSlotChanged(
        InspectorWidget *inspectorItem)
{
    Debug_Log("OnInspectorSlotChanged");
    XMLNode *xmlInfo = new XMLNode();
    m_prefabTempGameObject->Write(xmlInfo);
    Debug_Log("xmlInfo.ToString(): " << xmlInfo->ToString());
    AssetsManager::UpdateAsset(m_file.GetAbsolutePath(), *xmlInfo);
    delete xmlInfo;
}

GameObject *PrefabAssetFileInspectable::GetPrefabTempGameObject() const
{
    return m_prefabTempGameObject;
}



