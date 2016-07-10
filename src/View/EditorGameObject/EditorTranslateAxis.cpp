#include "EditorTranslateAxis.h"

#include "Toolbar.h"

EditorTranslateAxis::EditorTranslateAxis(EditorAxis::EditorAxisDirection dir,
                                         GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorTranslateAxisGroup" + EditorAxis::GetStringFromDir(dir));

    line = AddComponent<SingleLineRenderer>();
    line->SetDestiny(oAxisDirection);
    line->SetMaterial(material);
    line->SetLineWidth(2.0f);

    line->SetActivateGLStatesBeforeRenderingForSelectionFunction([]()
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
    Transform *camTransform = cam->gameObject->GetComponent<Transform>(); NONULL(camTransform);
    Transform *attTrans = attachedGameObject->GetComponent<Transform>(); NONULL(attTrans);
    Transform *transform = GetComponent<Transform>(); NONULL(transform);
    Vector3 wCamPos = camTransform->GetPosition();

    if (grabbed)
    {
        glm::vec2 sMouseDelta = Input::GetMouseDelta() * glm::vec2(1.0f, -1.0f);
        if (glm::length(sMouseDelta) > 0.0f)
        {
            Vector3 wAxisDir;
            if (Toolbar::GetInstance()->GetGlobalCoordsMode())
            {
                wAxisDir = oAxisDirection;
            }
            else
            {
                wAxisDir = transform->LocalToWorldDirection(oAxisDirection);
            }

            // Alignment
            Vector3 wAxisCenter = transform->GetPosition();
            glm::vec2 screenAxisDir = cam->WorldToScreenNDCPoint(wAxisCenter + wAxisDir) -
                                      cam->WorldToScreenNDCPoint(wAxisCenter);
            screenAxisDir = glm::normalize(screenAxisDir);
            float alignment = glm::dot(screenAxisDir, glm::normalize(sMouseDelta));
            //

            Vector3 worldMove = alignment * wAxisDir *
                                glm::length(sMouseDelta) *
                                Vector3::Distance(wCamPos, attTrans->GetPosition()) * 0.002f;
            attTrans->Translate(worldMove);
        }
    }
}

Renderer *EditorTranslateAxis::GetAxisRenderer() const
{
    return line;
}

