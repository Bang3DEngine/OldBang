#include "EditorRotateAxisGroup.h"

EditorRotateAxisGroup::EditorRotateAxisGroup()
{
    SetName("EditorRotateAxisGroup");

    axisX = new EditorRotateAxis(EditorAxis::EditorAxisDirection::X);
    axisY = new EditorRotateAxis(EditorAxis::EditorAxisDirection::Y);
    axisZ = new EditorRotateAxis(EditorAxis::EditorAxisDirection::Z);

    this->AddChild(axisX);
    this->AddChild(axisY);
    this->AddChild(axisZ);

    AddComponent<Transform>();
}

void EditorRotateAxisGroup::OnUpdate()
{
}
