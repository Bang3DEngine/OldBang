#include "EditorSelectionGameObject.h"

#include "WindowMain.h"

EditorSelectionGameObject::EditorSelectionGameObject(GameObject *attachedGameObject) :
    EditorGameObject("EditorSelectionGameObject")
{
    AddComponent<Transform>();

    axisGroup = new EditorAxisGroup(attachedGameObject);
    AddChild(axisGroup);

    //EditorBBox *bbox = new EditorBBox(attachedGameObject);
    //bbox->GetComponent<MeshRenderer>()->GetMaterial()->SetDiffuseColor(glm::vec4(1,0,0,1));
    //AddChild(bbox);
}
