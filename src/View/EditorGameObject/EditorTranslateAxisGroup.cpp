#include "EditorTranslateAxisGroup.h"

#include "EditorAxisPlane.h"
#include "EditorTranslateAxis.h"

EditorTranslateAxisGroup::EditorTranslateAxisGroup(GameObject *attachedGameObject)
    : EditorGameObject("EditorTranslateAxisGroup")
{
    m_axisX = new EditorTranslateAxis(EditorAxis::EditorAxisDirection::X, attachedGameObject);
    m_axisY = new EditorTranslateAxis(EditorAxis::EditorAxisDirection::Y, attachedGameObject);
    m_axisZ = new EditorTranslateAxis(EditorAxis::EditorAxisDirection::Z, attachedGameObject);
    m_axisX->SetParent(this);
    m_axisY->SetParent(this);
    m_axisZ->SetParent(this);

    //m_planeXY = new EditorAxisPlane(m_axisX, m_axisY);
    //m_planeXZ = new EditorAxisPlane(m_axisX, m_axisZ);
    //m_planeYZ = new EditorAxisPlane(m_axisY, m_axisZ);
    //m_planeXY->SetParent(this);
    //m_planeXZ->SetParent(this);
    //m_planeYZ->SetParent(this);
}

EditorTranslateAxisGroup::~EditorTranslateAxisGroup()
{
}

void EditorTranslateAxisGroup::OnUpdate()
{
}

