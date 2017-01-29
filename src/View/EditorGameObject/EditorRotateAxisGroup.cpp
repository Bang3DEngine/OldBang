#include "EditorRotateAxisGroup.h"

#include "Debug.h"
#include "Scene.h"
#include "Gizmos.h"
#include "Camera.h"
#include "Toolbar.h"
#include "Material.h"
#include "Transform.h"
#include "EditorScene.h"
#include "EditorCamera.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "CircleRenderer.h"
#include "EditorRotateAxis.h"

EditorRotateAxisGroup::EditorRotateAxisGroup(GameObject *attachedGameObject)
    : EditorGameObject("EditorRotateAxisGroup")
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
    m_circle->SetIsGizmo(true);
}

EditorRotateAxisGroup::~EditorRotateAxisGroup()
{
    delete m_circle->GetMaterial();
}

void EditorRotateAxisGroup::OnUpdate()
{
    EditorGameObject::OnUpdate();
}

void EditorRotateAxisGroup::OnDrawGizmosOverlay()
{
    EditorGameObject::OnDrawGizmosOverlay();

    Vector3 center = m_axisX->m_attachedGameObject->transform->GetPosition();
    float radius = 0.9f * m_axisX->m_circle->GetRadius() * transform->GetScale().x;

    // Billboard-scaling taken into account too
    Gizmos::SetColor(Color::Gray);
    Gizmos::SetReceivesLighting(false);
    Gizmos::SetPosition(center);
    Gizmos::SetBillboard();
    Gizmos::RenderCircle(radius);
}
