#include "EditorRotateAxis.h"

#include "Toolbar.h"

EditorRotateAxis::EditorRotateAxis(EditorAxis::EditorAxisDirection dir) : EditorAxis(dir)
{
    SetName("EditorRotateAxisGroup" + EditorAxis::GetStringFromDir(dir));

    material = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/linesRotationAxis.bmat");
    material = new Material(*material);

    circle = AddComponent<CircleRenderer>();
    circle->SetRadius(0.5f);
    circle->SetSegments(128);
    circle->SetLineWidth(3.0f);
    circle->SetMaterial(material);

    if(dir == EditorAxisDirection::X)
    {
        GetComponent<Transform>()->SetLocalEuler(Vector3::up * 90.0f);
    }
    else if(dir == EditorAxisDirection::Y)
    {
        GetComponent<Transform>()->SetLocalEuler(Vector3::right * 90.0f);
    }
    else
    {
        GetComponent<Transform>()->SetLocalEuler(Vector3::up * 180.0f);
    }

    this->SetRenderLayer(5);
}

EditorRotateAxis::~EditorRotateAxis()
{
}

void EditorRotateAxis::OnUpdate()
{
    EditorAxis::OnUpdate();

    // Obtain model, view and proj matrices, for next calculations
    Matrix4 pvm, projView, projMatrix, viewMatrix, modelMatrix;
    GetMatrices(pvm, projView, projMatrix, viewMatrix, modelMatrix);

    // Obtain mousePos in screen space for next calculations

    Camera *cam = Canvas::GetInstance()->GetCurrentScene()->GetCamera(); NONULL(cam);
    Transform *camTransform = cam->GetOwner()->GetComponent<Transform>(); NONULL(camTransform);

    GameObject *attachedGameObject = GetAttachedGameObject();
    Transform *attTrans = attachedGameObject->GetComponent<Transform>(); NONULL(attTrans);

    Sphere bSphere = attachedGameObject->GetBoundingSphere();
    float radius = bSphere.GetRadius() / 2.0f;
    material->GetShaderProgram()->SetUniformVec3("wCircleCenter", bSphere.GetCenter(), false);
    material->GetShaderProgram()->SetUniformVec3("boundingSphereRadius", radius, false);

    if(grabbed)
    {
        if(Input::GetMouseButtonDown(Input::MouseButton::MLeft))
        {
            // This will be done once every grab
            glm::vec2 sMousePos= Input::GetMouseCoords();
            sMousePos /= glm::vec2(Canvas::GetWidth(), Canvas::GetHeight());
            sMousePos.y = 1.0f - sMousePos.y;

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

            currentOAxisDirection = oAxisDirection;
        }


        // Process grabbing rotation movement
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

            // Avoids rotation trembling when not aligned at all
            if(glm::abs(alignment) > 0.2)
            {
                // Rotate the model
                Quaternion q = Quaternion::AngleAxis(rotationBoost * alignment,
                                                     currentOAxisDirection);

                if (Toolbar::GetInstance()->GetGlobalCoordsMode())
                {
                    attTrans->Rotate(q);
                }
                else
                {
                    attTrans->RotateLocal(q);
                }
            }
        }
    }
}

Renderer *EditorRotateAxis::GetAxisRenderer() const
{
    return circle;
}

