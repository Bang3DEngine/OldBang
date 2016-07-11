#include "EditorScaleAxisGroup.h"

#include "WindowMain.h"

EditorScaleAxisGroup::EditorScaleAxisGroup(GameObject *attachedGameObject) :
    EditorGameObject("EditorScaleAxisGroup")
{

    p_axisX = new EditorScaleAxis(EditorAxis::EditorAxisDirection::X, attachedGameObject);
    p_axisY = new EditorScaleAxis(EditorAxis::EditorAxisDirection::Y, attachedGameObject);
    p_axisZ = new EditorScaleAxis(EditorAxis::EditorAxisDirection::Z, attachedGameObject);

    AddChild(p_axisX);
    AddChild(p_axisY);
    AddChild(p_axisZ);

    AddComponent<Transform>();
}

EditorScaleAxisGroup::~EditorScaleAxisGroup()
{
}

void EditorScaleAxisGroup::OnUpdate()
{
}

