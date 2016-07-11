#include "EditorSelectionGameObject.h"

#include "WindowMain.h"

EditorSelectionGameObject::EditorSelectionGameObject(GameObject *attachedGameObject) :
    EditorGameObject("EditorSelectionGameObject")
{
    AddComponent<Transform>();

    p_axisGroup = new EditorAxisGroup(attachedGameObject);
    AddChild(p_axisGroup);

    //EditorBBox *bbox = new EditorBBox(attachedGameObject);
    //bbox->GetComponent<MeshRenderer>()->GetMaterial()->SetDiffuseColor(glm::vec4(1,0,0,1));
    //AddChild(bbox);
}
