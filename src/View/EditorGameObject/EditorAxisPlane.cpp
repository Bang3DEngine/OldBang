#include "Bang/EditorAxisPlane.h"

#include "Bang/Debug.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/EditorAxis.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/AssetsManager.h"

EditorAxisPlane::EditorAxisPlane(EditorAxis *axis1, EditorAxis *axis2)
    : m_axis1(axis1), m_axis2(axis2)
{
    m_material = AssetsManager::Load<Material>("Materials/G_Default.bmat", true);
    m_material = new Material(*m_material);

    m_planeRenderer = AddComponent<MeshRenderer>();
    m_planeRenderer->SetMesh(MeshFactory::GetPlane());
    m_planeRenderer->SetMaterial(m_material);

    SetTransformAccordingToAxis();

    GetHideFlags()->SetOn(HideFlag::HideAndDontSave);
}

EditorAxisPlane::~EditorAxisPlane()
{
    delete m_material;
}

void EditorAxisPlane::OnEditorUpdate()
{
}

void EditorAxisPlane::SetTransformAccordingToAxis()
{
    ENSURE(m_axis1); ENSURE(m_axis2);
    Vector3 pos = Vector3::Zero;
    Vector3 scale = Vector3::One;

    transform->SetLocalPosition(pos);
    transform->SetLocalScale(scale);
}
