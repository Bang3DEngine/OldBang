#include "PrefabAssetFileInspectable.h"

#include "Prefab.h"
#include "Inspector.h"
#include "GameObject.h"
#include "WindowMain.h"

PrefabAssetFileInspectable::PrefabAssetFileInspectable(const File &f) :
    m_file(f)
{
    Prefab *prefab = new Prefab();
    prefab->ReadXMLInfoFromString(f.GetContents());
    m_prefabTempGameObject = prefab->InstantiateWithoutStarting();
    delete prefab;
}

PrefabAssetFileInspectable::~PrefabAssetFileInspectable()
{
    WindowMain::GetInstance()->widgetInspector->Clear();
    delete m_prefabTempGameObject;
}

void PrefabAssetFileInspectable::ShowInInspector() const
{
    WindowMain::GetInstance()->widgetInspector->ShowGameObjectInfo(m_prefabTempGameObject);
}

void PrefabAssetFileInspectable::OnInspectorSlotChanged(InspectorWidget *inspectorItem)
{
    SaveToFile();
}

void PrefabAssetFileInspectable::SaveToFile()
{
    FileWriter::WriteToFile(m_file.GetAbsolutePath(),
                            m_prefabTempGameObject->GetXMLInfoString());
}



