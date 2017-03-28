#include "Bang/EditorTranslateAxisGroup.h"

#include "Bang/EditorAxisPlane.h"
#include "Bang/EditorTranslateAxis.h"

EditorTranslateAxisGroup::EditorTranslateAxisGroup(GameObject *attachedGameObject)
    : GameObject("EditorTranslateAxisGroup")
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

    AddHideFlag(HideFlags::HideAndDontSave);
}

EditorTranslateAxisGroup::~EditorTranslateAxisGroup()
{
}

void EditorTranslateAxisGroup::OnUpdate()
{
}

