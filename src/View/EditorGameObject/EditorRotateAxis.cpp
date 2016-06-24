#include "EditorRotateAxis.h"

#include "Toolbar.h"

EditorRotateAxis::EditorRotateAxis(EditorRotateAxisDirection dir)
{
    transform = AddComponent<Transform>();
    circle = AddComponent<CircleRenderer>();

    material = AssetsManager::GetAsset<Material>("res/Materials/linesRotationAxis.bmat");
    material = new Material(*material);
    circle->SetMaterial(material);

    std::string name;
    if(dir == EditorRotateAxisDirection::X)
    {
        lineColor = Vector3(1,0,0);
        oAxisDirection = Vector3(1,0,0);
        transform->SetRotation(0.0f, 90.0f, 0.0f);
        name = "EditorRotateAxisX";
    }
    else if (dir == EditorRotateAxisDirection::Y)
    {
        lineColor = Vector3(0,1,0);
        oAxisDirection = Vector3(0,1,0);
        transform->SetRotation(90.0f, 0.0f, 0.0f);
        name = "EditorRotateAxisY";
    }
    else
    {
        lineColor = Vector3(0,0,1);
        oAxisDirection = Vector3(0,0,1);
        transform->SetRotation(0.0f, 0.0f, 0.0f);
        name = "EditorRotateAxisZ";
    }

    material->SetDiffuseColor(glm::vec4(lineColor, 1));
    circle->SetSegments(64);
    SetName(name);

    material->SetDiffuseColor(glm::vec4(0,1,0, 1));
    circle->SetLineWidth(3.0f);

    this->SetRenderLayer(0);
}

EditorRotateAxis::~EditorRotateAxis()
{
    delete material;
}


void EditorRotateAxis::OnUpdate()
{
    // Obtain model, view and proj matrices, for next calculations
    Matrix4 projMatrix, viewMatrix, modelMatrix;

    Camera *cam = Canvas::GetInstance()->GetCurrentScene()->GetCamera();
    cam->GetProjectionMatrix(projMatrix);
    cam->GetViewMatrix(viewMatrix);

    GameObject *attachedGameObject = GetAttachedGameObject();
    Transform *goTrans = attachedGameObject->GetComponent<Transform>();
    attachedGameObject->GetComponent<Transform>()->GetModelMatrix(modelMatrix);

    Matrix4 projView = projMatrix * viewMatrix;
    Matrix4 pvm =  projView * modelMatrix;



    // Obtain mousePos in screen space for next calculations
    glm::vec2 sMousePos= Input::GetMouseCoords();
    sMousePos /= glm::vec2(Canvas::GetWidth(), Canvas::GetHeight());
    sMousePos.y = 1.0f - sMousePos.y;

    if(mouseIsOver && Input::GetMouseButtonDown(Input::MouseButton::MLeft))
    {
        // User has clicked on me!
        grabbed = true;

        // We are going to get the two anchor points !!!

        // Get the two circle's closer points to the selected point
        // by the user in screen space.
        circle->GetTwoClosestPointsInScreenSpace(
                    sMousePos , pvm,
                    &sAnchorPoint0, &anchorIndex0,
                    &sAnchorPoint1, &anchorIndex1);

        // achorIndex0 will always be less than anchorIndex1
        if (anchorIndex1 < anchorIndex0)
        {
            std::swap(sAnchorPoint0, sAnchorPoint1);
            std::swap(anchorIndex0, anchorIndex1);
        }

        if(Toolbar::GetInstance()->GetGlobalCoordsMode())
        {
            currentOAxisDirection =
                    Vector3((modelMatrix.Inversed() *
                             glm::vec4(oAxisDirection,0.0f)).xyz()).Normalized();
        }
        else
        {
            currentOAxisDirection = oAxisDirection;
        }
    }

    Sphere bSphere = attachedGameObject->GetBoundingSphere();
    float radius = bSphere.GetRadius() / 2.0f * 1.0f;
    circle->SetRadius(radius);

    material->GetShaderProgram()->SetUniformVec3("wCircleCenter", bSphere.GetCenter(), false);
    material->GetShaderProgram()->SetUniformVec3("boundingSphereRadius", radius, false);

    // Process grabbing rotation movement
    if (grabbed)
    {
        if (Input::GetMouseButtonUp(Input::MouseButton::MLeft))
        {
            grabbed = false;
            Input::SetMouseWrapping(false);
        }
        else
        {
            Input::SetMouseWrapping(true);

            // Normalized mouse movement in the last frame
            glm::vec2 sMouseDelta = Input::GetMouseDelta();
            if (glm::length(sMouseDelta) > 0.0f)
            {
                // sMouseDelta to screen space
                sMouseDelta /= glm::vec2(Canvas::GetWidth(), Canvas::GetHeight());
                sMouseDelta.y *= -1;

                // Get how aligned is the user movement with the anchor points
                glm::vec2 anchorPointsDir = glm::normalize(sAnchorPoint0 - sAnchorPoint1);
                glm::vec2 mouseDir = glm::normalize(sMouseDelta);
                float alignment = glm::dot(anchorPointsDir, mouseDir);

                // Rotate the model
                Quaternion q = Quaternion::AngleAxis(rotationBoost * alignment * -1.0f,
                                                     currentOAxisDirection);
                goTrans->Rotate(q);
            }
        }
    }

    // Process line color
    if (mouseIsOver)
    {
        if (grabbed)
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
        if (grabbed)
        {
            material->SetDiffuseColor(glm::vec4(lineColor, 1));
        }
        else
        {
            material->SetDiffuseColor(glm::vec4(lineColor * lineColorFadingNotHover, 1));
        }
    }
}

void EditorRotateAxis::OnMouseEnter()
{
    mouseIsOver = true;
}

void EditorRotateAxis::OnMouseExit()
{
    mouseIsOver = false;
}
