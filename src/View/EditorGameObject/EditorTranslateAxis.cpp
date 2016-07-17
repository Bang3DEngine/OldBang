#include "EditorTranslateAxis.h"

#include "Toolbar.h"

EditorTranslateAxis::EditorTranslateAxis(EditorAxis::EditorAxisDirection dir,
                                         GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorTranslateAxisGroup" + EditorAxis::GetStringFromDir(dir));

    p_line = AddComponent<SingleLineRenderer>();
    p_line->SetDestiny(m_oAxisDirection);
    p_line->SetMaterial(p_material);
    p_line->SetLineWidth(2.0f);

    p_line->SetActivateGLStatesBeforeRenderingForSelectionFunction([]()
        {
            glLineWidth(25.0f);
        }
    );
}

EditorTranslateAxis::~EditorTranslateAxis()
{

}


void EditorTranslateAxis::OnUpdate()
{
    EditorAxis::OnUpdate();

    Camera *cam = Canvas::GetInstance()->GetCurrentScene()->GetCamera(); NONULL(cam);
    Transform *camTransform = cam->gameObject->transform; NONULL(camTransform);
    GameObject *ago = p_attachedGameObject; NONULL(ago->transform);
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
                parentAxisDir = ago->parent->transform->WorldToLocalDirection(m_oAxisDirection);
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
            ago->transform->Translate(worldMove);
        }
    }
}

Renderer *EditorTranslateAxis::GetAxisRenderer() const
{
    return p_line;
}

