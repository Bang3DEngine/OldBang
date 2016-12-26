#include "EditorSelectionGameObject.h"

#include "EditorBBox.h"
#include "EditorAxisGroup.h"

EditorSelectionGameObject::EditorSelectionGameObject(GameObject *attachedGameObject) :
    EditorGameObject("EditorSelectionGameObject")
{
    m_axisGroup = new EditorAxisGroup(attachedGameObject);
    m_axisGroup->SetParent(this);

    //EditorBBox *bbox = new EditorBBox(attachedGameObject);
    //bbox->GetComponent<MeshRenderer>()->GetMaterial()->SetDiffuseColor(Vector4(1,0,0,1));
    //bbox->SetParent(this);
}

EditorSelectionGameObject::~EditorSelectionGameObject()
{

}
