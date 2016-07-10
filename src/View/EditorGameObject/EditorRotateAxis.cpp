#include "EditorRotateAxis.h"

#include "Toolbar.h"

EditorRotateAxis::EditorRotateAxis(EditorAxis::EditorAxisDirection dir,
                                   GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorRotateAxisGroup" + EditorAxis::GetStringFromDir(dir));

    material = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/linesRotationAxis.bmat");
    material = new Material(*material);

    circle = AddComponent<CircleRenderer>();
    circle->SetRadius(0.5f);
    circle->SetSegments(64);
    circle->SetLineWidth(2.0f);
    circle->SetMaterial(material);

    circle->SetActivateGLStatesBeforeRenderingForSelectionFunction([]()
        {
            glLineWidth(25.0f);
        }
    );

    if(dir == EditorAxisDirection::X)
    {
        GetComponent<Transform>()->SetLocalEuler(Vector3::up * 90.0f);
    }
    else if(dir == EditorAxisDirection::Y)
    {
        GetComponent<Transform>()->SetLocalEuler(Vector3::right * -90.0f);
    }
    else
    {
        GetComponent<Transform>()->SetLocalEuler(Vector3::up * 180.0f);
    }
}

EditorRotateAxis::~EditorRotateAxis()
{
}

void EditorRotateAxis::OnUpdate()
{
    EditorAxis::OnUpdate();

    // Obtain mousePos in screen space for next calculations
    Camera *cam = Canvas::GetInstance()->GetCurrentScene()->GetCamera(); NONULL(cam);
    Transform *camTransform = cam->gameObject->GetComponent<Transform>(); NONULL(camTransform);
    Transform *attTrans = attachedGameObject->GetComponent<Transform>(); NONULL(attTrans);
    Transform *transform = GetComponent<Transform>(); NONULL(transform);

    Matrix4 p, v, m;
    cam->GetProjectionMatrix(p);
    cam->GetViewMatrix(v);
    transform->GetModelMatrix(m);
    Matrix4 pvm =  p * v * m;

    if(grabbed)
    {
        if(Input::GetMouseButtonDown(Input::MouseButton::MLeft))
        {
            // This will be done once every grab
            glm::vec2 sMousePos= Input::GetMouseCoords();
            sMousePos /= glm::vec2(Canvas::GetWidth(), Canvas::GetHeight());
            sMousePos.y = 1.0f - sMousePos.y;
            sMousePos = sMousePos * 2.0f - 1.0f;

            // We are going to get the two anchor points !!!
            // Get the two circle's closer points to the selected point
            // by the user in screen space.
            int anchorIndex0, anchorIndex1;
            circle->GetTwoClosestPointsInScreenSpace(sMousePos , pvm,
                        &sAnchorPoint0, &anchorIndex0, &sAnchorPoint1, &anchorIndex1);

            // This is needed to properly compute the rotation
            if (anchorIndex1 < anchorIndex0)
            {
                std::swap(sAnchorPoint0, sAnchorPoint1);
                std::swap(anchorIndex0, anchorIndex1);
            }
        }

        glm::vec2 sMouseDelta = Input::GetMouseDelta() * glm::vec2(1.0f, -1.0f);
        if (glm::length(sMouseDelta) > 0.0f)
        {
            // Get how aligned is the user movement with the anchor points
            glm::vec2 anchorPointsDir = glm::normalize(sAnchorPoint1 - sAnchorPoint0);
            float alignment = glm::dot(anchorPointsDir, glm::normalize(sMouseDelta));

            // Avoids rotation trembling when not aligned at all
            Quaternion q = Quaternion::AngleAxis(rotationBoost * alignment, oAxisDirection);
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

    // Pass some uniforms to the shader that renders the rotation circles
    Sphere bSphere = attachedGameObject->GetBoundingSphere();
    float radius = bSphere.GetRadius() / 2.0f;
    material->GetShaderProgram()->SetUniformVec3("wCircleCenter", bSphere.GetCenter(), false);
    material->GetShaderProgram()->SetUniformVec3("boundingSphereRadius", radius, false);
}

Renderer *EditorRotateAxis::GetAxisRenderer() const
{
    return circle;
}

