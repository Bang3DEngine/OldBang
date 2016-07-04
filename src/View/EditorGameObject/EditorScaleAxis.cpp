#include "EditorScaleAxis.h"

#include "Toolbar.h"

EditorScaleAxis::EditorScaleAxis(EditorAxis::EditorAxisDirection dir,
                                 GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorScaleAxisGroup" + EditorAxis::GetStringFromDir(dir));

    line = AddComponent<SingleLineRenderer>();
    line->SetDestiny(oAxisDirection);
    line->SetMaterial(material);
    line->SetLineWidth(2.0f);

    line->SetActivateGLStatesBeforeRenderingForSelectionFunction([]()
        {
            glLineWidth(25.0f);
        }
    );

    this->SetRenderLayer(5);
}

EditorScaleAxis::~EditorScaleAxis()
{

}


void EditorScaleAxis::OnUpdate()
{
    EditorAxis::OnUpdate();

    // Obtain model, view and proj matrices, for next calculations
    Matrix4 pvm, projView, projMatrix, viewMatrix, modelMatrix;
    GetMatrices(pvm, projView, projMatrix, viewMatrix, modelMatrix);

    Camera *cam = Canvas::GetInstance()->GetCurrentScene()->GetCamera(); NONULL(cam);
    Transform *camTransform = cam->GetOwner()->GetComponent<Transform>(); NONULL(camTransform);
    Vector3 camPos = camTransform->GetPosition();

    Transform *attTrans = attachedGameObject->GetComponent<Transform>(); NONULL(attTrans);

    // Process grabbing movement
    if (grabbed)
    {
        // Normalized mouse movement in the last frame
        glm::vec2 mouseDelta = Input::GetMouseDelta() * glm::vec2(1.0f, -1.0f); // Invert y

        if (glm::length(mouseDelta) > 0.0f)
        {
            // Get axis in world space and eye space
            Matrix4 modelMatrix2;
            if (!Toolbar::GetInstance()->GetGlobalCoordsMode())
            {
                modelMatrix = Matrix4::identity;
                attTrans->GetModelMatrix(modelMatrix2);
            }
            else
            {
                attTrans->GetModelMatrix(modelMatrix);
                modelMatrix2 = Matrix4::identity;
            }

            glm::vec4 finalAxisDir = glm::normalize(modelMatrix * glm::vec4(oAxisDirection, 0.0f));
            glm::vec2 screenAxisDir = glm::normalize((projView * modelMatrix2 * glm::vec4(oAxisDirection, 0.0f)).xy());

            float alignment = glm::dot(screenAxisDir, glm::normalize(mouseDelta));
            Vector3 scaling = Vector3::one +
                              alignment * Vector3(finalAxisDir.xyz()) *
                              glm::length(mouseDelta) *
                              Vector3::Distance(camPos, attTrans->GetPosition()) * 0.001f;

            Logger_Log(alignment);
            Canvas::GetInstance()->GetCurrentScene()->DebugDrawScreenLine(glm::vec2(0,0),
                                                                          glm::vec2(0,0) + screenAxisDir,
                                                                          1.0f, 1.0f);

            //TODO: solve problem with negative scaling and depth :/
            attTrans->SetLocalScale(attTrans->GetLocalScale() * scaling);
        }
    }
}

Renderer *EditorScaleAxis::GetAxisRenderer() const
{
    return line;
}

