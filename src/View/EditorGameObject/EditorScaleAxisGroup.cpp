#include "EditorScaleAxisGroup.h"

#include "WindowMain.h"

EditorScaleAxisGroup::EditorScaleAxisGroup(GameObject *attachedGameObject) :
    EditorGameObject("EditorScaleAxisGroup")
{

    p_axisX = new EditorScaleAxis(EditorAxis::EditorAxisDirection::X, attachedGameObject);
    p_axisY = new EditorScaleAxis(EditorAxis::EditorAxisDirection::Y, attachedGameObject);
    p_axisZ = new EditorScaleAxis(EditorAxis::EditorAxisDirection::Z, attachedGameObject);

    p_axisX->SetParent(this);
    p_axisY->SetParent(this);
    p_axisZ->SetParent(this);
}

EditorScaleAxisGroup::~EditorScaleAxisGroup()
{
}

void EditorScaleAxisGroup::OnUpdate()
{
}

