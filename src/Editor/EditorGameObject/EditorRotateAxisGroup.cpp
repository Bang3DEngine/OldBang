#include "Bang/EditorRotateAxisGroup.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Gizmos.h"
#include "Bang/Camera.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/EditorScene.h"
#include "Bang/EditorCamera.h"
#include "Bang/MeshRenderer.h"
#include "Bang/SceneManager.h"
#include "Bang/CircleRenderer.h"
#include "Bang/EditorRotateAxis.h"
#include "Bang/CircleCulledRenderer.h"

EditorRotateAxisGroup::EditorRotateAxisGroup(GameObject *attachedGameObject)
    : GameObject("EditorRotateAxisGroup")
{
    m_axisX = new EditorRotateAxis(EditorAxis::EditorAxisDirection::X, attachedGameObject);
    m_axisY = new EditorRotateAxis(EditorAxis::EditorAxisDirection::Y, attachedGameObject);
    m_axisZ = new EditorRotateAxis(EditorAxis::EditorAxisDirection::Z, attachedGameObject);

    m_axisX->SetParent(this);
    m_axisY->SetParent(this);
    m_axisZ->SetParent(this);

    m_circle = AddComponent<CircleRenderer>();
    Material *matCopy = new Material( *(m_circle->GetMaterial()) );
    matCopy->SetDiffuseColor( Color::White );
    m_circle->SetMaterial(matCopy);

    GetHideFlags()->SetOn(HideFlag::HideAndDontSave);
}

EditorRotateAxisGroup::~EditorRotateAxisGroup()
{
    if (m_circle->GetMaterial()) { delete m_circle->GetMaterial(); }
}

void EditorRotateAxisGroup::OnEditorUpdate()
{
    GameObject::OnEditorUpdate();
}

void EditorRotateAxisGroup::OnDrawGizmos(bool depthed, bool overlay)
{
    GameObject::OnDrawGizmos(depthed, overlay);

    if (overlay)
    {
        Vector3 center = m_axisX->m_attachedGameObject->transform->GetPosition();
        float radius = 0.9f * m_axisX->m_circle->GetRadius() * transform->GetScale().x;

        // Billboard-scaling taken into account too
        Gizmos::SetColor(Color::Gray);
        Gizmos::SetReceivesLighting(false);
        Gizmos::SetPosition(center);
        Gizmos::SetBillboard();
        Gizmos::RenderCircle(radius);
    }
}
