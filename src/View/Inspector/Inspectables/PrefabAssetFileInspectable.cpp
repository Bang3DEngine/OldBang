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
    prefab.ReadFromFile( f.GetPath() );
    SetRelatedSerializableObject( prefab.InstantiateWithoutStarting() );

    GameObject *go = Object::SCast<GameObject>(GetRelatedSerializableObject());
    go->SetName( f.GetPath().GetName() );
    go->SetEnabled(true);
    OnInspectorSlotChanged(nullptr);
}

PrefabAssetFileInspectable::~PrefabAssetFileInspectable()
{
    OnInspectorSlotChanged(nullptr); // Update the file
}

void PrefabAssetFileInspectable::CloneInto(ICloneable *clone) const
{
    Inspectable<GameObject>::CloneInto(clone);
    PrefabAssetFileInspectable *pfinsp =
                        Object::SCast<PrefabAssetFileInspectable>(clone);
    pfinsp->m_file = m_file;
}

void PrefabAssetFileInspectable::OnInspectorSlotChanged(
                                             InspectorWidget *inspectorWidget)
{
    AssetsManager::UpdateAsset(m_file.GetPath(),
                               GetRelatedSerializableObject()->GetXMLInfo());
}


