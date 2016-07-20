#include "EditorRotateAxisGroup.h"

EditorRotateAxisGroup::EditorRotateAxisGroup(GameObject *attachedGameObject)
    : EditorGameObject("EditorRotateAxisGroup")
{
    p_axisX = new EditorRotateAxis(EditorAxis::EditorAxisDirection::X, attachedGameObject);
    p_axisY = new EditorRotateAxis(EditorAxis::EditorAxisDirection::Y, attachedGameObject);
    p_axisZ = new EditorRotateAxis(EditorAxis::EditorAxisDirection::Z, attachedGameObject);

    p_axisX->SetParent(this);
    p_axisY->SetParent(this);
    p_axisZ->SetParent(this);
}

void EditorRotateAxisGroup::OnUpdate()
{
}
