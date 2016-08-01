#include "EditorScaleAxis.h"

#include "Toolbar.h"

EditorScaleAxis::EditorScaleAxis(EditorAxis::EditorAxisDirection dir,
                                 GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorScaleAxisGroup" + EditorAxis::GetStringFromDir(dir));

    m_line = AddComponent<SingleLineRenderer>();
    m_line->SetDestiny(m_oAxisDirection);
    m_line->SetMaterial(m_material);
    m_line->SetLineWidth(2.0f);
    m_line->SetReceivesLighting(false);

    m_line->SetActivateGLStatesBeforeRenderingForSelectionFunction([]()
        {
            glLineWidth(25.0f);
        }
    );

    m_axisCap = MeshFactory::GetCubeEditorGameObject();
    m_axisCap->SetParent(this);
    m_axisCap->transform->SetLocalPosition(m_oAxisDirection);
    m_axisCap->transform->SetLocalScale(Vector3(0.2f));
    m_axisCap->GetComponent<MeshRenderer>()->SetMaterial(m_material);
    m_axisCap->GetComponent<MeshRenderer>()->SetReceivesLighting(true);
}

EditorScaleAxis::~EditorScaleAxis()
{

}


void EditorScaleAxis::OnUpdate()
{
    EditorAxis::OnUpdate();

    Camera *cam = Canvas::GetInstance()->GetCurrentScene()->GetCamera(); NONULL(cam);
    Transform *camTransform = cam->gameObject->transform; NONULL(camTransform);
    GameObject *ago = m_attachedGameObject; NONULL(ago->transform);
    Vector3 wCamPos = camTransform->GetPosition();

    if (m_grabbed)
    {
        glm::vec2 sMouseDelta = Input::GetMouseDelta() * glm::vec2(1.0f, -1.0f);
        if (glm::length(sMouseDelta) > 0.0f)
        {
            Vector3 wAxisDir, parentAxisDir;
            if (Toolbar::GetInstance()->IsInGlobalCoordsMode())
            {
                wAxisDir = m_oAxisDirection;
                parentAxisDir = ago->transform->WorldToObjectDirection(m_oAxisDirection);
            }
            else
            {
                wAxisDir = ago->transform->ObjectToWorldDirection(m_oAxisDirection);
                parentAxisDir = m_oAxisDirection;
            }
            wAxisDir.Normalize();
            parentAxisDir.Normalize();

            // Alignment
            Vector3 wAxisCenter = transform->GetPosition();
            glm::vec2 screenAxisDir = cam->WorldToScreenNDCPoint(wAxisCenter + wAxisDir) -
                                      cam->WorldToScreenNDCPoint(wAxisCenter);
            screenAxisDir = glm::normalize(screenAxisDir);
            float alignment = glm::dot(screenAxisDir, glm::normalize(sMouseDelta));
            //

            Vector3 scaling = alignment *
                              parentAxisDir.Abs() *
                              glm::length(sMouseDelta) *
                              Vector3::Distance(wCamPos, ago->transform->GetPosition()) * 0.0002f;

            //TODO: solve problem with negative scaling and depth :/
            ago->transform->SetLocalScale(ago->transform->GetLocalScale() + scaling);
        }
    }
}

void EditorScaleAxis::OnDrawGizmosNoDepth()
{
    m_line->Render();
    m_axisCap->GetComponent<Renderer>()->Render();
}

Renderer *EditorScaleAxis::GetAxisRenderer() const
{
    return m_line;
}

