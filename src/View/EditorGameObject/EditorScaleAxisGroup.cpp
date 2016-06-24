#include "EditorScaleAxisGroup.h"

#include "WindowMain.h"

EditorScaleAxisGroup::EditorScaleAxisGroup() : EditorGameObject("EditorScaleAxisGroup")
{
    axisX = new EditorScaleAxis(EditorAxis::EditorAxisDirection::X);
    axisY = new EditorScaleAxis(EditorAxis::EditorAxisDirection::Y);
    axisZ = new EditorScaleAxis(EditorAxis::EditorAxisDirection::Z);

    AddChild(axisX);
    AddChild(axisY);
    AddChild(axisZ);

    AddComponent<Transform>();
}

EditorScaleAxisGroup::~EditorScaleAxisGroup()
{
}

void EditorScaleAxisGroup::OnUpdate()
{
}

