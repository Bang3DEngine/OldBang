#include "EditorTranslateAxis.h"

EditorTranslateAxis::EditorTranslateAxis(EditorTranslateAxis::EditorTranslateAxisDirection dir) :
    EditorGameObject()
{
    transform = AddComponent<Transform>();
    line = AddComponent<SingleLineRenderer>();

    material = new Material(*line->GetMaterial());
    line->SetMaterial(material);

    std::string name;
    if(dir == EditorTranslateAxisDirection::X)
    {
        lineColor = Vector3(1,0,0);
        axisDirection = Vector3(1,0,0);
        name = "EditorTranslateAxisX";
    }
    else if (dir == EditorTranslateAxisDirection::Y)
    {
        lineColor = Vector3(0,1,0);
        axisDirection = Vector3(0,1,0);
        name = "EditorTranslateAxisY";
    }
    else
    {
        lineColor = Vector3(0,0,1);
        axisDirection = Vector3(0,0,1);
        name = "EditorTranslateAxisZ";
    }

    material->SetDiffuseColor(glm::vec4(lineColor, 1));
    line->SetDestiny(axisDirection * 3.0f);
    line->SetLineWidth(5.0f);
    SetName(name);

    this->SetRenderLayer(5);
}


void EditorTranslateAxis::OnUpdate()
{
    // Process line color
    if(mouseIsOver)
    {
        if(grabbed)
        {
            material->SetDiffuseColor(glm::vec4(lineColor, 1));
        }
        else
        {
            material->SetDiffuseColor(glm::vec4(lineColor * lineColorFadingNotGrabbed, 1));
        }
    }
    else
    {
        if(grabbed)
        {
            material->SetDiffuseColor(glm::vec4(lineColor, 1));
        }
        else
        {
            material->SetDiffuseColor(glm::vec4(lineColor * lineColorFadingNotHover, 1));
        }
    }


    // Process grabbing movement
    if(grabbed)
    {
        Input::SetMouseWrapping(true);
        if(Input::GetMouseButtonUp(Input::MouseButton::MLeft))
        {
            grabbed = false;
            Input::SetMouseWrapping(false);
        }
        else
        {
            GameObject *attachedGameObject = GetAttachedGameObject();
            // Normalized mouse movement in the last frame
            glm::vec2 mouseDelta = Input::GetMouseDelta() * glm::vec2(1.0f, -1.0f); // Invert y

            if(glm::length(mouseDelta) > 0.0f)
            {
                // Obtain model, view and proj matrices
                Matrix4 projMatrix, viewMatrix, modelMatrix;
                Camera *cam = Canvas::GetInstance()->GetCurrentScene()->GetCamera();
                cam->GetProjectionMatrix(projMatrix);
                cam->GetViewMatrix(viewMatrix);
                GameObject *axisGroup = GetParent();
                axisGroup->GetComponent<Transform>()->GetModelMatrix(modelMatrix);
                Matrix4 projView = projMatrix * viewMatrix;

                //Get axis in world space and eye space
                glm::vec4 worldAxisDir = glm::normalize(modelMatrix *
                                                        glm::vec4(axisDirection, 0.0f));
                glm::vec2 screenAxisDir = glm::normalize((projView * worldAxisDir).xy());

                // Move the GameObject, depending on how aligned is
                // the mouse movement vs the axis in screen space (what user sees)
                float alignment = glm::dot(screenAxisDir, glm::normalize(mouseDelta));
                Vector3 camPos = cam->GetOwner()->GetComponent<Transform>()->GetPosition();
                Transform *goTrans = attachedGameObject->GetComponent<Transform>();
                Vector3 worldMove = alignment * Vector3(worldAxisDir.xyz()) *
                                    glm::length(mouseDelta) *
                                    Vector3::Distance(camPos, goTrans->GetPosition()) * 0.002f;
                goTrans->Translate(worldMove);
            }
        }
    }
}

void EditorTranslateAxis::OnMouseEnter()
{
    mouseIsOver = true;
    if(Input::GetMouseButtonDown(Input::MouseButton::MLeft))
    {
        grabbed = true;
    }
}

void EditorTranslateAxis::OnMouseExit()
{
    mouseIsOver = false;
}
