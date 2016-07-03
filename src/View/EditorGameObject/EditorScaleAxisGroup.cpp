#include "EditorScaleAxisGroup.h"

#include "WindowMain.h"

EditorScaleAxisGroup::EditorScaleAxisGroup(GameObject *attachedGameObject) :
    EditorGameObject("EditorScaleAxisGroup")
{

    axisX = new EditorScaleAxis(EditorAxis::EditorAxisDirection::X, attachedGameObject);
    axisY = new EditorScaleAxis(EditorAxis::EditorAxisDirection::Y, attachedGameObject);
    axisZ = new EditorScaleAxis(EditorAxis::EditorAxisDirection::Z, attachedGameObject);

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

