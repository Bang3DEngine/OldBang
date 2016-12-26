#include "EditorScaleAxisGroup.h"

#include "EditorAxis.h"
#include "EditorScaleAxis.h"

EditorScaleAxisGroup::EditorScaleAxisGroup(GameObject *attachedGameObject) :
    EditorGameObject("EditorScaleAxisGroup")
{

    m_axisX = new EditorScaleAxis(EditorAxis::EditorAxisDirection::X, attachedGameObject);
    m_axisY = new EditorScaleAxis(EditorAxis::EditorAxisDirection::Y, attachedGameObject);
    m_axisZ = new EditorScaleAxis(EditorAxis::EditorAxisDirection::Z, attachedGameObject);

    m_axisX->SetParent(this);
    m_axisY->SetParent(this);
    m_axisZ->SetParent(this);
}

EditorScaleAxisGroup::~EditorScaleAxisGroup()
{
}

void EditorScaleAxisGroup::OnUpdate()
{
}

