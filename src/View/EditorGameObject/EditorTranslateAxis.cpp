#include "EditorTranslateAxis.h"

#include "Toolbar.h"

EditorTranslateAxis::EditorTranslateAxis(EditorAxis::EditorAxisDirection dir,
                                         GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorTranslateAxisGroup" + EditorAxis::GetStringFromDir(dir));

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
        m_axisCap->transform->SetLocalEuler(Vector3(90.0f, 0.0f, 0.0f));
    }
    m_axisCap->transform->SetLocalPosition(m_oAxisDirection);
    m_axisCap->transform->SetLocalScale(Vector3(1,2,1) * 0.15f);
    m_axisCap->GetComponent<MeshRenderer>()->SetMaterial(m_material);
    m_axisCap->GetComponent<MeshRenderer>()->SetReceivesLighting(true);
    m_axisCap->SetRenderLayer(5);
}

EditorTranslateAxis::~EditorTranslateAxis()
{

}


void EditorTranslateAxis::OnUpdate()
{
    EditorAxis::OnUpdate();

    Camera *cam = Canvas::GetInstance()->GetCurrentScene()->GetCamera(); NONULL(cam);
    Transform *camTransform = cam->gameObject->transform;
    GameObject *ago = m_attachedGameObject;
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
                parentAxisDir = ago->transform->WorldToLocalDirection(m_oAxisDirection);
            }
            else
            {
                wAxisDir = ago->transform->ObjectToWorldDirection(m_oAxisDirection);
                parentAxisDir = ago->transform->ObjectToLocalDirection(m_oAxisDirection);
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

            Vector3 worldMove = alignment *
                                parentAxisDir *
                                glm::length(sMouseDelta) *
                                Vector3::Distance(wCamPos, ago->transform->GetPosition()) *
                                Time::deltaTime * 0.02f;
            ago->transform->TranslateLocal(worldMove);
        }
    }
}

Renderer *EditorTranslateAxis::GetAxisRenderer() const
{
    return m_line;
}

