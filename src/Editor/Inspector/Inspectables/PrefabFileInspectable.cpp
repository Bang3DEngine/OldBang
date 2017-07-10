#include "Bang/PrefabFileInspectable.h"

#include "Bang/Debug.h"
#include "Bang/Prefab.h"
#include "Bang/Inspector.h"
#include "Bang/GameObject.h"
#include "Bang/EditorWindow.h"
#include "Bang/AssetsManager.h"
#include "Bang/InspectorWidget.h"
#include "Bang/FileReferencesManager.h"

PrefabFileInspectable::PrefabFileInspectable()
{
}

PrefabFileInspectable::PrefabFileInspectable(const BFile &f) :
    m_file(f)
{
    Prefab prefab;
    prefab.ReadFromFile( f.GetPath() );
    SetRelatedSerializableObject( prefab.InstantiateWithoutStarting() );

    GameObject *go = Object::SCast<GameObject>(GetSerializableObject());
    go->SetName( f.GetPath().GetName() );
    go->SetEnabled(true);
    OnInspectorSlotChanged(nullptr);
}

PrefabFileInspectable::~PrefabFileInspectable()
{
    OnInspectorSlotChanged(nullptr); // Update the file
}

void PrefabFileInspectable::CloneInto(ICloneable *clone) const
{
    Inspectable<GameObject>::CloneInto(clone);
    PrefabFileInspectable *pfinsp =
                        Object::SCast<PrefabFileInspectable>(clone);
    pfinsp->m_file = m_file;
}

void PrefabFileInspectable::OnInspectorSlotChanged(
                                             InspectorWidget *inspectorWidget)
{
    AssetsManager::UpdateAsset(m_file.GetPath(),
                               GetSerializableObject()->GetXMLInfo());
}


