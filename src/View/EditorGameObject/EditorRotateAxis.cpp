#include "EditorRotateAxis.h"

#include "Toolbar.h"

EditorRotateAxis::EditorRotateAxis(EditorAxis::EditorAxisDirection dir,
                                   GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorRotateAxisGroup" + EditorAxis::GetStringFromDir(dir));

    m_material = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/D2G_LineRotationAxis.bmat");
    m_material = new Material(*m_material);

    m_circle = AddComponent<CircleRenderer>();
    m_circle->SetRadius(0.5f);
    m_circle->SetSegments(64);
    m_circle->SetLineWidth(2.0f);
    m_circle->SetMaterial(m_material);
    m_circle->SetReceivesLighting(false);

    m_circle->SetActivateGLStatesBeforeRenderingForSelectionFunction([]()
        {
            glLineWidth(25.0f);
        }
    );

    if (dir == EditorAxisDirection::X)
    {
        transform->SetLocalEuler(Vector3::up * 90.0f);
    }
    else if (dir == EditorAxisDirection::Y)
    {
        transform->SetLocalEuler(Vector3::right * -90.0f);
    }
    else
    {
        transform->SetLocalEuler(Vector3::up * 180.0f);
    }

    m_oAxisDirection.z *= -1;
}

EditorRotateAxis::~EditorRotateAxis()
{
}

void EditorRotateAxis::OnUpdate()
{
    EditorAxis::OnUpdate();

    // Obtain mousePos in screen space for next calculations
    Camera *cam = Canvas::GetInstance()->GetCurrentScene()->GetCamera(); NONULL(cam);
    GameObject *ago = m_attachedGameObject;

    Matrix4 p, v, m;
    cam->GetProjectionMatrix(&p);
    cam->GetViewMatrix(&v);
    transform->GetModelMatrix(&m);
    Matrix4 pvm =  p * v * m;

    if (m_grabbed)
    {
        if (Input::GetMouseButtonDown(Input::MouseButton::MLeft))
        {
            // This will be done once every grab
            Vector2 sMousePos= Input::GetMouseCoords();
            sMousePos /= Vector2(Canvas::GetWidth(), Canvas::GetHeight());
            sMousePos.y = 1.0f - sMousePos.y;
            sMousePos = sMousePos * 2.0f - 1.0f;

            // We are going to get the two anchor points !!!
            // Get the two circle's closer points to the selected point
            // by the user in screen space.
            int anchorIndex0, anchorIndex1;
            m_circle->GetTwoClosestPointsInScreenSpace(sMousePos , pvm,
                        &m_sAnchorPoint0, &anchorIndex0, &m_sAnchorPoint1, &anchorIndex1);

            // This is needed to properly compute the rotation
            if (anchorIndex1 < anchorIndex0)
            {
                std::swap(m_sAnchorPoint0, m_sAnchorPoint1);
                std::swap(anchorIndex0, anchorIndex1);
            }
        }

        Vector2 sMouseDelta = Input::GetMouseDelta() * Vector2(1.0f, -1.0f);
        if (sMouseDelta.Length() > 0.0f)
        {
            // Get how aligned is the user movement with the anchor points
            Vector2 anchorPointsDir = Vector2(m_sAnchorPoint1 - m_sAnchorPoint0).Normalized();
            float alignment = Vector2::Dot(anchorPointsDir, Vector2(sMouseDelta).Normalized());

            float rotAngle = alignment * c_rotationBoost;

            // Avoids rotation trembling when not aligned at all
            Vector3 parentAxisDir;
            if (Toolbar::GetInstance()->IsInGlobalCoordsMode())
            {
                parentAxisDir = ago->transform->WorldToObjectDirection(m_oAxisDirection);
            }
            else
            {
                parentAxisDir = m_oAxisDirection;
            }

            Quaternion q = Quaternion::AngleAxis(rotAngle, parentAxisDir);
            ago->transform->RotateLocal(q);
        }
    }

    // Pass some uniforms to the shader that renders the rotation circles
    Sphere bSphere = m_attachedGameObject->GetBoundingSphere();
    float radius = bSphere.GetRadius() / 2.0f;
    m_material->GetShaderProgram()->SetUniformVec3("B_world_circleCenter", bSphere.GetCenter(), false);
    m_material->GetShaderProgram()->SetUniformVec3("B_boundingSphereRadius", radius, false);
}

void EditorRotateAxis::OnDrawGizmosNoDepth()
{
    m_circle->Render();
}

Renderer *EditorRotateAxis::GetAxisRenderer() const
{
    return m_circle;
}

