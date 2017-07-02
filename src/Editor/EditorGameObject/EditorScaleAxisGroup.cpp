#include "Bang/EditorScaleAxisGroup.h"

#include "Bang/Material.h"
#include "Bang/EditorAxis.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/AssetsManager.h"
#include "Bang/EditorScaleAxis.h"

EditorScaleAxisGroup::EditorScaleAxisGroup(GameObject *attachedGameObject) :
    GameObject("EditorScaleAxisGroup")
{

    m_axisX = new EditorScaleAxis(
                EditorAxis::EditorAxisDirection::X, attachedGameObject);
    m_axisY = new EditorScaleAxis(
                EditorAxis::EditorAxisDirection::Y, attachedGameObject);
    m_axisZ = new EditorScaleAxis(
                EditorAxis::EditorAxisDirection::Z, attachedGameObject);
    m_axisCentral = new EditorScaleAxis(
                EditorAxis::EditorAxisDirection::Central, attachedGameObject);


    m_axisX->SetParent(this);
    m_axisY->SetParent(this);
    m_axisZ->SetParent(this);
    m_axisCentral->SetParent(this);

    GetHideFlags()->SetOn(HideFlag::HideAndDontSave);
}

EditorScaleAxisGroup::~EditorScaleAxisGroup()
{
}

void EditorScaleAxisGroup::OnEditorUpdate()
{
}
