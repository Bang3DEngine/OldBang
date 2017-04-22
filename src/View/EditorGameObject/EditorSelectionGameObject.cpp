#include "Bang/EditorSelectionGameObject.h"

#include "Bang/EditorBBox.h"
#include "Bang/EditorAxisGroup.h"
#include "Bang/EditorRectTransformGizmo.h"

EditorSelectionGameObject::EditorSelectionGameObject(GameObject *attachedGameObject) :
    GameObject("EditorSelectionGameObject")
{
    m_axisGroup = new EditorAxisGroup(attachedGameObject);
    m_axisGroup->SetParent(this);

    m_rectTransformGizmo = new EditorRectTransformGizmo(attachedGameObject);
    m_rectTransformGizmo->SetParent(this);

    //EditorBBox *bbox = new EditorBBox(attachedGameObject);
    //bbox->GetComponent<MeshRenderer>()->GetMaterial()->
    //                            SetDiffuseColor(Vector4(1,0,0,1));
    //bbox->SetParent(this);

    GetHideFlags()->SetOn(HideFlag::HideAndDontSave);
}

EditorSelectionGameObject::~EditorSelectionGameObject()
{

}
