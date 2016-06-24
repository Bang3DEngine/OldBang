#include "EditorTranslateAxis.h"

#include "Toolbar.h"

EditorTranslateAxis::EditorTranslateAxis(EditorAxis::EditorAxisDirection dir) :
    EditorAxis(dir)
{
    line = AddComponent<SingleLineRenderer>();
    line->SetDestiny(oAxisDirection);
    line->SetMaterial(material);
    line->SetLineWidth(5.0f);

    this->SetRenderLayer(5);
}

EditorTranslateAxis::~EditorTranslateAxis()
{

}


void EditorTranslateAxis::OnUpdate()
{
    EditorAxis::OnUpdate();

    // Obtain model, view and proj matrices, for next calculations
    Matrix4 pvm, projView, projMatrix, viewMatrix, modelMatrix;
    GetMatrices(pvm, projView, projMatrix, viewMatrix, modelMatrix);

    // Process grabbing movement
    if(grabbed)
    {
        // Normalized mouse movement in the last frame
        glm::vec2 mouseDelta = Input::GetMouseDelta() * glm::vec2(1.0f, -1.0f); // Invert y

        if(glm::length(mouseDelta) > 0.0f)
        {
            Camera *cam = Canvas::GetInstance()->GetCurrentScene()->GetCamera();

            // Get axis in world space and eye space
            if(Toolbar::GetInstance()->GetGlobalCoordsMode())
            {
                modelMatrix = Matrix4::identity;
            }

            glm::vec4 worldAxisDir = glm::normalize(modelMatrix *
                                                    glm::vec4(oAxisDirection, 0.0f));
            glm::vec2 screenAxisDir = glm::normalize((projView * worldAxisDir).xy());

            // Move the GameObject, depending on how aligned is
            // the mouse movement vs the axis in screen space (what user sees)
            float alignment = glm::dot(screenAxisDir, glm::normalize(mouseDelta));
            Vector3 camPos = cam->GetOwner()->GetComponent<Transform>()->GetPosition();
            Transform *goTrans = GetAttachedGameObject()->GetComponent<Transform>();
            Vector3 worldMove = alignment * Vector3(worldAxisDir.xyz()) *
                                glm::length(mouseDelta) *
                                Vector3::Distance(camPos, goTrans->GetPosition()) * 0.002f;
            goTrans->Translate(worldMove);
        }
    }
}

Renderer *EditorTranslateAxis::GetAxisRenderer() const
{
    return line;
}

