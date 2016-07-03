#include "EditorRotateAxisGroup.h"

EditorRotateAxisGroup::EditorRotateAxisGroup(GameObject *attachedGameObject)
    : EditorGameObject("EditorRotateAxisGroup")
{
    axisX = new EditorRotateAxis(EditorAxis::EditorAxisDirection::X, attachedGameObject);
    axisY = new EditorRotateAxis(EditorAxis::EditorAxisDirection::Y, attachedGameObject);
    axisZ = new EditorRotateAxis(EditorAxis::EditorAxisDirection::Z, attachedGameObject);

    this->AddChild(axisX);
    this->AddChild(axisY);
    this->AddChild(axisZ);

    AddComponent<Transform>();
}

void EditorRotateAxisGroup::OnUpdate()
{
}
