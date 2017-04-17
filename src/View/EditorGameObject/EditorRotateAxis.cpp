#include "Bang/EditorRotateAxis.h"

#include "Bang/Input.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Screen.h"
#include "Bang/Camera.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/EditorState.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"
#include "Bang/CircleCulledRenderer.h"

EditorRotateAxis::EditorRotateAxis(EditorAxis::EditorAxisDirection dir,
                                   GameObject *attachedGameObject)
    : EditorAxis(dir, attachedGameObject)
{
    SetName("EditorRotateAxisGroup" + EditorAxis::GetStringFromDir(dir));

    m_circle = AddComponent<CircleCulledRenderer>();
    m_circle->SetLineWidth(2.0f);

    m_circle->SetMaterial(m_material);
    m_circle->SetDepthLayer(Renderer::DepthLayer::DepthLayerGizmos);
    m_circle->SetBindForSelectionFunction([]()
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

void EditorRotateAxis::OnEditorUpdate()
{
    EditorAxis::OnEditorUpdate();

    // Obtain mousePos in screen space for next calculations
    Camera *cam = SceneManager::GetActiveScene()->GetCamera(); ENSURE(cam);
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

    m_circle->OnUpdate();
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
