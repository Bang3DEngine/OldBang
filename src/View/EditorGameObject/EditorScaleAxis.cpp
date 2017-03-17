#include "EditorScaleAxis.h"

#include "Debug.h"
#include "Scene.h"
#include "Input.h"
#include "Camera.h"
#include "Matrix4.h"
#include "Material.h"
#include "Renderer.h"
#include "Transform.h"
#include "MeshFactory.h"
#include "EditorState.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "SingleLineRenderer.h"

EditorScaleAxis::EditorScaleAxis(EditorAxis::EditorAxisDirection dir,
                                 GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorScaleAxisGroup" + EditorAxis::GetStringFromDir(dir));

    // Line Renderer configuration
    m_line = AddComponent<SingleLineRenderer>();
    m_line->SetDestiny(m_oAxisDirection);
    m_line->SetMaterial(m_material);
    m_line->SetLineWidth(2.0f);
    m_line->SetDepthLayer(Renderer::DepthLayer::DepthLayerGizmos);
    m_line->SetBindForSelectionFunction([]()
        {
            GL::SetLineWidth(25.0f); // Easier grab
        }
    );

    // Scale axis cap configuration (the cube at the end)
    m_axisCap = MeshFactory::GetCubeGameObject();
    m_axisCap->AddHideFlag(HideFlags::HideAndDontSave);
    m_axisCap->SetParent(this);
    m_axisCap->transform->SetLocalPosition(m_oAxisDirection);
    m_axisCap->transform->SetLocalScale(Vector3(0.2f));
    m_axisCap->GetComponent<MeshRenderer>()->SetMaterial(m_material);
    m_axisCap->GetComponent<MeshRenderer>()->
            SetDepthLayer(Renderer::DepthLayer::DepthLayerGizmos);

    if (dir == EditorAxis::EditorAxisDirection::Central)
    {
        ConfigureCentralAxis();
    }

    AddHideFlag(HideFlags::HideAndDontSave);
}

EditorScaleAxis::~EditorScaleAxis()
{

}


void EditorScaleAxis::OnUpdate()
{
    EditorAxis::OnUpdate();

    Camera *cam = SceneManager::GetActiveScene()->GetCamera(); ASSERT(cam);
    Transform *camTransform = cam->gameObject->transform; ASSERT(camTransform);
    GameObject *ago = m_attachedGameObject; ASSERT(ago->transform);
    Vector3 wCamPos = camTransform->GetPosition();

    SetEnabled(EditorState::GetCurrentTransformMode() ==
               EditorState::TransformMode::Scale);
    if (m_grabbed)
    {
        Vector2 sMouseDelta = Input::GetMouseDelta() * Vector2(1.0f, -1.0f);
        if (sMouseDelta.Length() > 0.0f)
        {
            Vector3 wAxisDir, parentAxisDir;
            if (EditorState::IsUsingGlobalCoords())
            {
                wAxisDir = m_oAxisDirection;
                parentAxisDir = ago->transform->WorldToLocalDirection(m_oAxisDirection);
            }
            else
            {
                wAxisDir = ago->transform->LocalToWorldDirection(m_oAxisDirection);
                parentAxisDir = m_oAxisDirection;
            }
            wAxisDir.Normalize();
            parentAxisDir.Normalize();

            // Alignment
            Vector3 wAxisCenter = transform->GetPosition();
            Vector2 screenAxisDir = cam->WorldToScreenNDCPoint(wAxisCenter + wAxisDir) -
                                      cam->WorldToScreenNDCPoint(wAxisCenter);
            screenAxisDir.Normalize();
            float alignment = Vector2::Dot(screenAxisDir, sMouseDelta.Normalized());
            //

            Vector3 scaling = alignment *
                              parentAxisDir.Abs() *
                              sMouseDelta.Length() *
                              Vector3::Distance(wCamPos, ago->transform->GetPosition()) * 0.002f;

            scaling *= 1.0f / ago->parent->transform->GetScale();
            //TODO: solve problem with negative scaling and depth :/
            ago->transform->SetLocalScale(ago->transform->GetLocalScale() + scaling);
        }
    }
}

void EditorScaleAxis::OnDrawGizmos(bool depthed, bool overlay)
{
    EditorAxis::OnDrawGizmos(depthed, overlay);

    if (overlay)
    {
        Gizmos::Render(m_line);
        Gizmos::Render(m_axisCap->GetComponent<Renderer>());
    }
}

Renderer *EditorScaleAxis::GetAxisRenderer() const
{
    return m_line;
}

void EditorScaleAxis::ConfigureCentralAxis()
{
    m_line->SetDestiny(Vector3::Zero); // i.e. no line
    m_axisCap->transform->SetLocalPosition(Vector3::Zero); // Center
}

