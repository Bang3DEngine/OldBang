#include "EditorRotateAxisGroup.h"

EditorRotateAxisGroup::EditorRotateAxisGroup(GameObject *attachedGameObject)
    : EditorGameObject("EditorRotateAxisGroup")
{
    p_axisX = new EditorRotateAxis(EditorAxis::EditorAxisDirection::X, attachedGameObject);
    p_axisY = new EditorRotateAxis(EditorAxis::EditorAxisDirection::Y, attachedGameObject);
    p_axisZ = new EditorRotateAxis(EditorAxis::EditorAxisDirection::Z, attachedGameObject);

    this->AddChild(p_axisX);
    this->AddChild(p_axisY);
    this->AddChild(p_axisZ);

    AddComponent<Transform>();
}

void EditorRotateAxisGroup::OnUpdate()
{
}
