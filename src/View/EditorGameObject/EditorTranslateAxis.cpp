#include "EditorTranslateAxis.h"

#include "Debug.h"
#include "Scene.h"
#include "Camera.h"
#include "Toolbar.h"
#include "Matrix4.h"
#include "Material.h"
#include "Transform.h"
#include "Transform.h"
#include "MeshFactory.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "EditorGameObject.h"
#include "SingleLineRenderer.h"

EditorTranslateAxis::EditorTranslateAxis(EditorAxis::EditorAxisDirection dir,
                                         GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorTranslateAxis" + EditorAxis::GetStringFromDir(dir));

    m_line = AddComponent<SingleLineRenderer>();
    m_line->SetDestiny(m_oAxisDirection);
    m_line->SetMaterial(m_material);
    m_line->SetLineWidth(2.0f);
    m_line->SetReceivesLighting(false);
    m_line->SetIsGizmo(true);
    m_line->SetDepthLayer(Renderer::DepthLayer::DepthLayerGizmosOverlay);
    m_line->SetActivateGLStatesBeforeRenderingForSelectionFunction([]()
        {
            glLineWidth(25.0f); // Easier grab
        }
    );

    m_axisCap = MeshFactory::GetConeEditorGameObject();
    m_axisCap->SetParent(this);
    if (dir == EditorAxis::EditorAxisDirection::X)
    {
        m_axisCap->transform->SetLocalEuler(Vector3(90.0f, 90.0f, 0.0f));
    }
    else if (dir == EditorAxis::EditorAxisDirection::Y)
    {
        m_axisCap->transform->SetLocalEuler(Vector3(0.0f, 0.0f, 0.0f));
    }
    else if (dir == EditorAxis::EditorAxisDirection::Z)
    {
        m_axisCap->transform->SetLocalEuler(Vector3(-90.0f, 0.0f, 0.0f));
    }
    m_axisCap->transform->SetLocalPosition(m_oAxisDirection);
    m_axisCap->transform->SetLocalScale(Vector3(1,2,1) * 0.25f);
    m_axisCap->GetComponent<MeshRenderer>()->SetMaterial(m_material);
    m_axisCap->GetComponent<MeshRenderer>()->SetReceivesLighting(true);
    m_axisCap->GetComponent<MeshRenderer>()->SetIsGizmo(true);
    m_axisCap->GetComponent<MeshRenderer>()->
            SetDepthLayer(Renderer::DepthLayer::DepthLayerGizmosOverlay);
}

EditorTranslateAxis::~EditorTranslateAxis()
{
}


void EditorTranslateAxis::OnUpdate()
{
    EditorAxis::OnUpdate();

    Camera *cam = SceneManager::GetActiveScene()->GetCamera(); NONULL(cam);
    Transform *camTransform = cam->gameObject->transform;
    GameObject *ago = m_attachedGameObject;
    Vector3 wCamPos = camTransform->GetPosition();

    SetEnabled(Toolbar::GetInstance()->GetSelectedTransformMode() ==
               Toolbar::TransformMode::Translate);
    if (m_grabbed)
    {
        Vector2 sMouseDelta = Input::GetMouseDelta() * Vector2(1.0f, -1.0f);
        if (sMouseDelta.Length() > 0.0f)
        {
            Vector3 wAxisDir, parentAxisDir;
            if (Toolbar::GetInstance()->IsInGlobalCoordsMode())
            {
                wAxisDir = m_oAxisDirection;
                if (ago->parent)
                {
                    parentAxisDir = ago->parent->transform->WorldToLocalDirection(m_oAxisDirection);
                }
            }
            else
            {
                wAxisDir = ago->transform->LocalToWorldDirection(m_oAxisDirection);
                parentAxisDir = ago->transform->LocalToParentDirection(m_oAxisDirection);
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

            Vector3 worldMove = alignment *
                                parentAxisDir *
                                sMouseDelta.Length() *
                                Vector3::Distance(wCamPos, ago->transform->GetPosition()) * 0.002f;
            worldMove *= 1.0f / ago->parent->transform->GetScale();
            ago->transform->TranslateLocal(worldMove);
        }
    }
}

void EditorTranslateAxis::OnDrawGizmosOverlay()
{
    EditorAxis::OnDrawGizmosOverlay();

    Gizmos::Render(m_line);
    Gizmos::Render(m_axisCap->GetComponent<Renderer>());
}

Renderer *EditorTranslateAxis::GetAxisRenderer() const
{
    return m_line;
}

