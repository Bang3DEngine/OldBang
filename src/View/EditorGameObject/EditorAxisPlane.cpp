#include "EditorAxisPlane.h"

#include "Debug.h"
#include "Material.h"
#include "Transform.h"
#include "EditorAxis.h"
#include "MeshFactory.h"
#include "MeshRenderer.h"
#include "AssetsManager.h"

EditorAxisPlane::EditorAxisPlane(EditorAxis *axis1, EditorAxis *axis2)
    : m_axis1(axis1), m_axis2(axis2)
{
    m_material = AssetsManager::Load<Material>("Materials/D2G_Default.bmat", true);
    m_material = new Material(*m_material);

    m_planeRenderer = AddComponent<MeshRenderer>();
    m_planeRenderer->SetMesh(MeshFactory::GetPlane());
    m_planeRenderer->SetMaterial(m_material);

    SetTransformAccordingToAxis();
}

EditorAxisPlane::~EditorAxisPlane()
{
    delete m_material;
}

void EditorAxisPlane::OnUpdate()
{
}

void EditorAxisPlane::SetTransformAccordingToAxis()
{
    ASSERT(m_axis1); ASSERT(m_axis2);
    Vector3 pos = Vector3::Zero;
    Vector3 scale = Vector3::One;

    transform->SetLocalPosition(pos);
    transform->SetLocalScale(scale);
}
