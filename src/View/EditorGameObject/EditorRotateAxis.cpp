#include "EditorRotateAxis.h"

#include "Input.h"
#include "Debug.h"
#include "Scene.h"
#include "Screen.h"
#include "Camera.h"
#include "Material.h"
#include "Transform.h"
#include "EditorState.h"
#include "SceneManager.h"
#include "ShaderProgram.h"
#include "AssetsManager.h"
#include "CircleRenderer.h"

EditorRotateAxis::EditorRotateAxis(EditorAxis::EditorAxisDirection dir,
                                   GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorRotateAxisGroup" + EditorAxis::GetStringFromDir(dir));

    m_circle = AddComponent<CircleCulledRenderer>();
    m_circle->SetLineWidth(2.0f);

    m_circle->SetMaterial(m_material);
    m_circle->SetDepthLayer(Renderer::DepthLayer::DepthLayerGizmos);
    m_circle->SetOnRenderingStartsForSelectionFunction([]()
        {
            GL::SetLineWidth(10.0f); // Easier grab
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

    AddHideFlag(HideFlags::HideAndDontSave);
}

EditorRotateAxis::~EditorRotateAxis()
{
}

void EditorRotateAxis::OnUpdate()
{
    EditorAxis::OnUpdate();

    // Obtain mousePos in screen space for next calculations
    Camera *cam = SceneManager::GetActiveScene()->GetCamera(); ASSERT(cam);
    GameObject *ago = m_attachedGameObject;

    SetEnabled(EditorState::GetCurrentTransformMode() ==
               EditorState::TransformMode::Rotate);
    if (m_grabbed)
    {
        if (Input::GetMouseButtonDown(Input::MouseButton::Left))
        {
            // This will be done once every grab
            Vector2 sMousePos = Input::GetMouseCoords();
            sMousePos /= Screen::GetSize();
            sMousePos.y = 1.0f - sMousePos.y;
            sMousePos = sMousePos * 2.0f - 1.0f;

            // We are going to get the two anchor points !!!
            // Get the two circle's closer points to the selected point
            // by the user in screen space.
            int anchorIndex0, anchorIndex1;
            m_circle->GetTwoClosestPointsInScreenSpace(sMousePos,
                                                       &m_sAnchorPoint0,
                                                       &anchorIndex0,
                                                       &m_sAnchorPoint1,
                                                       &anchorIndex1);

            Debug::DrawScreenLine(Vector2::Zero, m_sAnchorPoint0, Color::Green);
            Debug::DrawScreenLine(Vector2::Zero, m_sAnchorPoint1, Color::Red);

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
            Vector2 anchorPointsDir =
                    Vector2(m_sAnchorPoint1 - m_sAnchorPoint0).Normalized();
            float alignment =
               Vector2::Dot(anchorPointsDir, Vector2(sMouseDelta).Normalized());

            float rotAngle = alignment * c_rotationBoost;

            // Avoids rotation trembling when not aligned at all
            Vector3 parentAxisDir;
            if (EditorState::IsUsingGlobalCoords())
            {
                parentAxisDir = ago->parent->transform->WorldToLocalDirection(
                                                            m_oAxisDirection);
            }
            else
            {
                parentAxisDir = m_oAxisDirection;
            }

            parentAxisDir.Normalize();

            Quaternion q = Quaternion::AngleAxis(rotAngle, parentAxisDir);
            if (EditorState::IsUsingGlobalCoords())
            {
                ago->transform->SetLocalRotation(
                          q.Normalized() * ago->transform->GetLocalRotation());
            }
            else
            {
                ago->transform->RotateLocal(q);
            }
        }
    }
}

void EditorRotateAxis::OnDrawGizmos(bool depthed, bool overlay)
{
    EditorAxis::OnDrawGizmos(depthed, overlay);

    if (overlay)
    {
        Gizmos::Render(m_circle);
    }
}

Renderer *EditorRotateAxis::GetAxisRenderer() const
{
    return m_circle;
}

/////////////////////////////////

void CircleCulledRenderer::OnUpdate()
{
    LineRenderer::OnUpdate();
    GeneratePoints();
    BindPointsToVAO();
}

void CircleCulledRenderer::GeneratePoints()
{
    m_points.Clear();
    const float radius = GetRadius();
    const int segments = 64;

    Matrix4 localToWorld;
    transform->GetLocalToWorldMatrix(&localToWorld);
    const Vector3 center = transform->GetPosition();

    Camera *cam = SceneManager::GetActiveScene()->GetCamera();
    const Vector3 camPos = cam->transform->GetPosition();

    const float step = (2.0f * Math::PI) / segments;
    for (int i = 0;  i < segments; ++i)
    {
        Vector3 pLocal = GetCircleLocalPoint(step * i) * radius;
        Vector3 pWorld = (localToWorld * Vector4(pLocal, 1)).xyz();
        float dot = Vector3::Dot( (pWorld - center).Normalized(),
                                  (camPos - center).Normalized());
        if (dot >= -0.1)
        {
            Vector3 pLocalNext = GetCircleLocalPoint(step * (i+1)) * radius;
            m_points.PushBack(pLocal);
            m_points.PushBack(pLocalNext);
        }
    }
}

void CircleCulledRenderer::GetTwoClosestPointsInScreenSpace(
        const Vector2 &sOrigin,
        Vector2 *p0, int *i0,
        Vector2 *p1, int *i1) const
{
    Matrix4 localToWorld;
    transform->GetLocalToWorldMatrix(&localToWorld);

    float d0, d1; d0 = d1 = 99999.9f;
    Vector3 latestPoint = m_points[0];
    for (int i = 0; i < m_points.Size() - 1; ++i)
    {
        Vector3 pLocal = m_points[i];
        if (latestPoint == pLocal) { continue; }

        Vector3 worldP = (localToWorld * Vector4(pLocal,1)).xyz();
        Camera *cam = SceneManager::GetActiveScene()->GetCamera();
        Vector2 sP = cam->WorldToScreenNDCPoint(worldP);

        float d = Vector2::Distance(sP, sOrigin);
        if (d < d0)
        {
            *p1 = *p0;
            *i1 = *i0;
            d1 = d0;

            *p0 = sP;
            *i0 = i;
            d0 = d;
        }
        else if (d < d1)
        {
            *p1 = sP;
            *i1 = i;
            d1 = d;
        }
        latestPoint = pLocal;
    }
}

Vector3 CircleCulledRenderer::GetCircleLocalPoint(float angle) const
{
    return Vector3(Math::Cos(angle), Math::Sin(angle), 0);
}

float CircleCulledRenderer::GetRadius() const
{
    return 0.5f;
}
