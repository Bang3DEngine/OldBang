#include "Bang/EditorScaleAxis.h"

#include "Bang/Math.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/Matrix4.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/MeshFactory.h"
#include "Bang/EditorState.h"
#include "Bang/MeshRenderer.h"
#include "Bang/SceneManager.h"
#include "Bang/SingleLineRenderer.h"

EditorScaleAxis::EditorScaleAxis(EditorAxis::EditorAxisDirection dir,
                                 GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorScaleAxisGroup" + EditorAxis::GetStringFromDir(dir));

    // Line Renderer configuration
    m_line = AddComponent<SingleLineRenderer>();
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
    m_axisCap->GetHideFlags()->SetOn(HideFlag::HideAndDontSave);
    m_axisCap->SetParent(this);
    m_axisCap->GetComponent<MeshRenderer>()->SetMaterial(m_material);
    m_axisCap->GetComponent<MeshRenderer>()->
            SetDepthLayer(Renderer::DepthLayer::DepthLayerGizmos);

    if (dir == EditorAxis::EditorAxisDirection::Central)
    {
        m_line->SetDestiny(Vector3::Zero);
        m_axisCap->transform->SetLocalPosition(Vector3::Zero);
    }

    GetHideFlags()->SetOn(HideFlag::HideAndDontSave);
}

EditorScaleAxis::~EditorScaleAxis()
{

}


void EditorScaleAxis::OnEditorUpdate()
{
    EditorAxis::OnEditorUpdate();

    Camera *cam = SceneManager::GetActiveScene()->GetCamera(); ENSURE(cam);
    Transform *camTransform = cam->gameObject->transform; ENSURE(camTransform);
    GameObject *ago = m_attachedGameObject; ENSURE(ago->transform);
    Vector3 wCamPos = camTransform->GetPosition();

    SetEnabled(EditorState::GetCurrentTransformMode() ==
               EditorState::TransformMode::Scale);
    Vector2 sMouseDelta = Input::GetMouseDelta() * Vector2(1.0f, -1.0f);
    if (m_grabbed && sMouseDelta.Length() > 0.0f)
    {
        Vector3 wAxisDir, localAxisDir;
        if (EditorState::IsUsingGlobalCoords())
        {
            wAxisDir = m_oAxisDirection;
            localAxisDir = ago->transform->WorldToLocalVector(m_oAxisDirection);
        }
        else
        {
            wAxisDir = ago->transform->LocalToWorldVector(m_oAxisDirection);
            localAxisDir = m_oAxisDirection;
        }
        wAxisDir.Normalize();
        localAxisDir.Normalize();

        // Alignment
        Vector3 wAxisCenter = transform->GetPosition();
        Vector2 screenAxisDir = cam->WorldToScreenNDCPoint(wAxisCenter + wAxisDir) -
                                  cam->WorldToScreenNDCPoint(wAxisCenter);
        screenAxisDir.Normalize();
        float alignment = Vector2::Dot(screenAxisDir, sMouseDelta.Normalized());
        //

        float d = Vector3::Distance(wCamPos, ago->transform->GetPosition());
        Vector3 scaling = alignment *
                          localAxisDir.Abs() *
                          sMouseDelta.Length() *
                          d * 0.002f;
        scaling *= 1.0f / ago->parent->transform->GetScale();

        Vector3 newLocalScale = ago->transform->GetLocalScale() + scaling;
        newLocalScale.x = Math::Max(newLocalScale.x, 0.01f);
        newLocalScale.y = Math::Max(newLocalScale.y, 0.01f);
        newLocalScale.z = Math::Max(newLocalScale.z, 0.01f);

        ago->transform->SetLocalScale(newLocalScale);

        Vector3 originalScale = m_originalScale;
        originalScale.x = Math::Max(originalScale.x, 0.01f);
        originalScale.y = Math::Max(originalScale.y, 0.01f);
        originalScale.z = Math::Max(originalScale.z, 0.01f);

        Vector3 scaleRatio = ago->transform->GetLocalScale() / originalScale;
        scaleRatio.x = Math::Max(scaleRatio.x, 0.2f);
        scaleRatio.y = Math::Max(scaleRatio.y, 0.2f);
        scaleRatio.z = Math::Max(scaleRatio.z, 0.2f);

        if (m_oAxisDirection != Vector3::One) // If its not the middle axis
        {
            Vector3 axis = EditorState::IsUsingGlobalCoords() ?
                                              wAxisDir : localAxisDir;
            m_line->SetDestiny(axis * scaleRatio);
            m_axisCap->transform->SetLocalPosition(axis * scaleRatio);
        }
    }

    if (!m_grabbed)
    {
        m_originalScale = ago->transform->GetLocalScale();
        if (m_oAxisDirection != Vector3::One) // If its not the middle axis
        {
            m_line->SetDestiny(m_oAxisDirection);
            m_axisCap->transform->SetLocalPosition(m_oAxisDirection);
        }
        m_axisCap->transform->SetLocalScale(Vector3(0.2f));
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

