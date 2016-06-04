#include "EditorAxis.h"

EditorAxis::EditorAxis(EditorAxis::EditorAxisDirection dir) :
    EditorGameObject()
{
    transform = AddComponent<Transform>();
    line = AddComponent<LineRenderer>();

    material = new Material(*line->GetMaterial());
    line->SetMaterial(material);

    std::string name;
    if(dir == EditorAxisDirection::X)
    {
        lineColor = Vector3(1,0,0);
        axisDirection = Vector3(1,0,0);
        name = "EditorAxisX";
    }
    else if (dir == EditorAxisDirection::Y)
    {
        lineColor = Vector3(0,1,0);
        axisDirection = Vector3(0,1,0);
        name = "EditorAxisY";
    }
    else
    {
        lineColor = Vector3(0,0,1);
        axisDirection = Vector3(0,0,1);
        name = "EditorAxisZ";
    }

    material->SetDiffuseColor(glm::vec4(lineColor, 1));
    line->SetDestiny(axisDirection * 3.0f);
    line->SetLineWidth(5.0f);
    SetName(name);

    this->SetRenderLayer(5);
}


void EditorAxis::OnUpdate()
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
            glm::vec2 mouseAxis = Input::GetMouseAxis() * glm::vec2(1.0f, -1.0f); // Invert y

            // Obtain model, view and proj matrices to know how
            // the axis is in eye space
            Matrix4 modelMatrix, viewMatrix, projMatrix;
            attachedGameObject->GetComponent<Transform>()->GetMatrix(modelMatrix);
            Canvas::GetInstance()->GetCurrentScene()->GetCamera()->GetViewMatrix(viewMatrix);
            Canvas::GetInstance()->GetCurrentScene()->GetCamera()->GetProjectionMatrix(projMatrix);

            //Get axis in world space and eye space
            glm::vec4 worldDir = glm::normalize(glm::vec4(axisDirection, 0.0f));
            //glm::normalize((modelMatrix * glm::vec4(axisDirection, 0.0f)));
            glm::vec4 eyeDir = glm::normalize((viewMatrix * worldDir));
            glm::vec4 screenDir4 = glm::normalize((projMatrix * eyeDir));
            glm::vec2 screenDir = glm::vec2(screenDir4.x, screenDir4.y);

            // Move the GameObject, depending on how aligned is
            // the mouse movement vs the axis in screen space (what user sees)
            float alignment = glm::dot(screenDir, mouseAxis);
            attachedGameObject->
                    GetComponent<Transform>()->Translate(alignment * Vector3(worldDir.x,
                                                                             worldDir.y,
                                                                             worldDir.z));
        }
    }
}

void EditorAxis::OnMouseEnter()
{
    mouseIsOver = true;
    if(Input::GetMouseButtonDown(Input::MouseButton::MLeft))
    {
        grabbed = true;
    }
}

void EditorAxis::OnMouseExit()
{
    mouseIsOver = false;
}
