#include "EditorRotateAxis.h"

#include "Debug.h"
#include "Scene.h"
#include "Screen.h"
#include "Toolbar.h"
#include "Material.h"
#include "Transform.h"
#include "SceneManager.h"
#include "ShaderProgram.h"
#include "AssetsManager.h"
#include "CircleRenderer.h"

EditorRotateAxis::EditorRotateAxis(EditorAxis::EditorAxisDirection dir,
                                   GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorRotateAxisGroup" + EditorAxis::GetStringFromDir(dir));

    m_circle = AddComponent<CircleRenderer>();
    m_circle->SetRadius(0.5f);
    m_circle->SetSegments(64);
    m_circle->SetLineWidth(2.0f);

    delete m_material; // Delete default EditorAxis material
    Material *mat = AssetsManager::Load<Material>(
                "Materials/D2G_RotationAxisLine.bmat", true);
    m_material = new Material(*mat);
    m_circle->SetMaterial(m_material);

    m_circle->SetReceivesLighting(false);
    m_circle->SetIsGizmo(true);
    m_circle->SetDepthLayer(Renderer::DepthLayer::DepthLayerGizmosOverlay);
    m_circle->SetActivateGLStatesBeforeRenderingForSelectionFunction([]()
        {
            glLineWidth(10.0f); // Easier grab
        }
    );

    if (dir == EditorAxisDirection::X)
    {
        transform->SetLocalEuler(Vector3::Up * 90.0f);
    }
    else if (dir == EditorAxisDirection::Y)
    {
        transform->SetLocalEuler(Vector3::Right * -90.0f);
    }
    else
    {
        transform->SetLocalEuler(Vector3::Up * 180.0f);
    }
}

EditorRotateAxis::~EditorRotateAxis()
{
}

void EditorRotateAxis::OnUpdate()
{
    EditorAxis::OnUpdate();

    // Obtain mousePos in screen space for next calculations
    Camera *cam = SceneManager::GetActiveScene()->GetCamera(); EXISTS(cam);
    GameObject *ago = m_attachedGameObject;

    SetEnabled(Toolbar::GetInstance()->GetSelectedTransformMode() ==
               Toolbar::TransformMode::Rotate);
    if (m_grabbed)
    {
        if (Input::GetMouseButtonDown(Input::MouseButton::MLeft))
        {
            // This will be done once every grab
            Vector2 sMousePos= Input::GetMouseCoords();
            sMousePos /= Screen::GetSize();
            sMousePos.y = 1.0f - sMousePos.y;
            sMousePos = sMousePos * 2.0f - 1.0f;

            // We are going to get the two anchor points !!!
            // Get the two circle's closer points to the selected point
            // by the user in screen space.
            int anchorIndex0, anchorIndex1;
            m_circle->GetTwoClosestPointsInScreenSpace(sMousePos,
                                                       &m_sAnchorPoint0, &anchorIndex0,
                                                       &m_sAnchorPoint1, &anchorIndex1);

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
                parentAxisDir = ago->parent->transform->WorldToLocalDirection(m_oAxisDirection);
            }
            else
            {
                parentAxisDir = m_oAxisDirection;
            }

            parentAxisDir.Normalize();

            Quaternion q = Quaternion::AngleAxis(rotAngle, parentAxisDir);
            if (Toolbar::GetInstance()->IsInGlobalCoordsMode())
            {
                ago->transform->SetLocalRotation(q.Normalized() * ago->transform->GetLocalRotation());
            }
            else
            {
                ago->transform->RotateLocal(q);
            }
        }
    }

    // Pass some uniforms to the shader that renders the rotation circles
    Sphere bSphere = m_attachedGameObject->GetBoundingSphere();
    float radius = bSphere.GetRadius() / 2.0f;
    m_material->GetShaderProgram()->SetUniformVec3("B_world_circleCenter", bSphere.GetCenter());
    m_material->GetShaderProgram()->SetUniformVec3("B_boundingSphereRadius", radius);
}

void EditorRotateAxis::OnDrawGizmosOverlay()
{
    EditorAxis::OnDrawGizmosOverlay();

    Gizmos::Render(m_circle);
}

Renderer *EditorRotateAxis::GetAxisRenderer() const
{
    return m_circle;
}

