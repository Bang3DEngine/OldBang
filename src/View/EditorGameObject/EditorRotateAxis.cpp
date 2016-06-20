#include "EditorRotateAxis.h"

EditorRotateAxis::EditorRotateAxis(EditorRotateAxisDirection dir)
{
    transform = AddComponent<Transform>();
    circle = AddComponent<CircleRenderer>();

    material = AssetsManager::GetAsset<Material>("res/Materials/linesRotationAxis.bmat");
    circle->SetMaterial(material);

    std::string name;
    if(dir == EditorRotateAxisDirection::X)
    {
        lineColor = Vector3(1,0,0);
        axisDirection = Vector3(1,0,0);
        transform->SetRotation(0.0f, 90.0f, 0.0f);
        name = "EditorRotateAxisX";
    }
    else if (dir == EditorRotateAxisDirection::Y)
    {
        lineColor = Vector3(0,1,0);
        axisDirection = Vector3(0,1,0);
        transform->SetRotation(90.0f, 0.0f, 0.0f);
        name = "EditorRotateAxisY";
    }
    else
    {
        lineColor = Vector3(0,0,1);
        axisDirection = Vector3(0,0,1);
        transform->SetRotation(0.0f, 0.0f, 0.0f);
        name = "EditorRotateAxisZ";
    }

    glEnable(GL_CULL_FACE);
    material->SetDiffuseColor(glm::vec4(lineColor, 1));
    circle->SetSegments(6);
    SetName(name);

    circle->SetLineWidth(3.0f);

    this->SetRenderLayer(0);
}


void EditorRotateAxis::OnUpdate()
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

    GameObject *attachedGameObject = GetAttachedGameObject();
    Sphere bSphere = attachedGameObject->GetBoundingSphere();
    float radius = bSphere.GetRadius() / 2.0f * 1.0f;
    circle->SetRadius(radius);

    material->GetShaderProgram()->SetUniformVec3("wCircleCenter", bSphere.GetCenter(), false);
    material->GetShaderProgram()->SetUniformVec3("boundingSphereRadius", radius, false);


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

            // Normalized mouse movement in the last frame
            glm::vec2 mouseDelta = Input::GetMouseDelta();
            if(glm::length(mouseDelta) > 0.0f)
            {
                // Obtain model, view and proj matrices
                Matrix4 projMatrix, viewMatrix, modelMatrix;
                Camera *cam = Canvas::GetInstance()->GetCurrentScene()->GetCamera();
                cam->GetProjectionMatrix(projMatrix);
                cam->GetViewMatrix(viewMatrix);
                Vector3 wCamPos = cam->GetOwner()->GetComponent<Transform>()->GetPosition();

                GetComponent<Transform>()->GetModelMatrix(modelMatrix);
                Matrix4 projView = projMatrix * viewMatrix;
                Matrix4 pvm =  projView * modelMatrix;
                Transform *goTrans = attachedGameObject->GetComponent<Transform>();

                glm::vec2 sSelectedPoint2 = Input::GetMouseCoords();
                sSelectedPoint2 /= glm::vec2(Canvas::GetWidth(), Canvas::GetHeight());

                Vector3 sSelectedPoint = Vector3(sSelectedPoint2.x, sSelectedPoint2.y, 0.0f);

                // Get the two circle's closer points to the selected point by the user.
                const std::vector<Vector3>& points = circle->GetPoints();
                Vector3 closestCirclePoint0, closestCirclePoint1;
                float closestDistance0 = 99999.9f, closestDistance1 = 99999.9f;
                int index0, index1, i = 0;
                for (Vector3 _p : points)
                {
                    Vector3 p = Vector3((modelMatrix * glm::vec4(_p, 1.0f)).xyz());
                    glm::vec4 pScreen4 = pvm * glm::vec4(_p, 1.0f);
                    Vector3 pScreen = Vector3(pScreen4.xyz() / pScreen4.w);
                    pScreen = pScreen * 0.5f + 0.5f;
                    pScreen.y = 1.0f - pScreen.y;
                    pScreen.z = 0.0f;
                    float d = pScreen.Distance(sSelectedPoint);
                    Logger_Log(i << ": " << sSelectedPoint <<  ",   " << pScreen << ", " << d);
                    if(d < closestDistance0)
                    {
                        closestDistance1 = closestDistance0;
                        closestCirclePoint1 = closestCirclePoint0;
                        index1 = index0;

                        closestDistance0 = d;
                        index0 = i;
                        closestCirclePoint0 = p;
                    }
                    else if(d < closestDistance1)
                    {
                        index1 = i;
                        closestDistance1 = d;
                        closestCirclePoint1 = p;
                    }
                    ++i;
                }

                Logger_Log("Closest points: " << index0 << "(" << closestDistance0 << "), " << index1 << "(" << closestDistance1 << ")");
                glm::vec2 closestPointsDir = glm::normalize(closestCirclePoint0.ToGlmVec3() -
                                                            closestCirclePoint1.ToGlmVec3()).xy();
                glm::vec2 selectedDir = glm::normalize(mouseDelta);
                float alignment = glm::dot(closestPointsDir, selectedDir);
                //Logger_Log("Alignment: " << alignment);

                Canvas::GetCurrentScene()->DebugDrawLine(bSphere.GetCenter(), closestCirclePoint0,
                                                         3.0f, 0.1f);
                Canvas::GetCurrentScene()->DebugDrawLine(bSphere.GetCenter(), closestCirclePoint1,
                                                         3.0f, 0.1f);
/*
                glm::vec2 screenCircleCenter = (projView * glm::vec4(bSphere.GetCenter(), 1.0f)).xy();
                screenCircleCenter /= screenCircleCenter.w;
                screenCircleCenter /= glm::vec2(Canvas::GetWidth(), Canvas::GetHeight());
*/
            }
        }
    }
}

void EditorRotateAxis::OnMouseEnter()
{
    mouseIsOver = true;
    if(Input::GetMouseButtonDown(Input::MouseButton::MLeft))
    {
        grabbed = true;
    }
}

void EditorRotateAxis::OnMouseExit()
{
    mouseIsOver = false;
}
