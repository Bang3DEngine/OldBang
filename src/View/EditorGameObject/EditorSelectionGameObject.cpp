#include "EditorSelectionGameObject.h"

#include "WindowMain.h"

EditorSelectionGameObject::EditorSelectionGameObject() :
    EditorGameObject("EditorSelectionGameObject")
{
    AddComponent<Transform>();

    axis = new EditorAxis();
    AddChild(axis);

    bbox = new EditorBBox();
    //AddChild(bbox);

    EditorBBox *bbox2 = new EditorBBox();
    bbox2->GetComponent<MeshRenderer>()->GetMaterial()->SetDiffuseColor(glm::vec4(1,0,0,1));
    AddChild(bbox2);
}
