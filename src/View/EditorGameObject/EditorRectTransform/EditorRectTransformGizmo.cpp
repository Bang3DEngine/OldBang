#include "EditorRectTransformGizmo.h"

#include "Debug.h"
#include "EditorState.h"
#include "RectTransform.h"
#include "EditorRectTransformAnchorGizmo.h"
#include "EditorRectTransformCornerGizmo.h"

EditorRectTransformGizmo::EditorRectTransformGizmo(
        GameObject *attachedGameObject)
    : EditorGizmo(attachedGameObject)
{
    m_leftTopCorner = new EditorRectTransformCornerGizmo(
                EditorRectTransformCornerGizmo::CornerPosition::TopLeft,
                attachedGameObject);

    m_rightTopCorner = new EditorRectTransformCornerGizmo(
                EditorRectTransformCornerGizmo::CornerPosition::TopRight,
                attachedGameObject);

    m_leftBotCorner = new EditorRectTransformCornerGizmo(
                EditorRectTransformCornerGizmo::CornerPosition::BotLeft,
                attachedGameObject);

    m_rightBotCorner = new EditorRectTransformCornerGizmo(
                EditorRectTransformCornerGizmo::CornerPosition::BotRight,
                attachedGameObject);

    m_centerCorner = new EditorRectTransformCornerGizmo(
                EditorRectTransformCornerGizmo::CornerPosition::Center,
                attachedGameObject);

    m_leftTopAnchor = new EditorRectTransformAnchorGizmo(
                EditorRectTransformAnchorGizmo::AnchorPosition::TopLeft,
                attachedGameObject);

    m_rightTopAnchor = new EditorRectTransformAnchorGizmo(
                EditorRectTransformAnchorGizmo::AnchorPosition::TopRight,
                attachedGameObject);

    m_leftBotAnchor = new EditorRectTransformAnchorGizmo(
                EditorRectTransformAnchorGizmo::AnchorPosition::BotLeft,
                attachedGameObject);

    m_rightBotAnchor = new EditorRectTransformAnchorGizmo(
                EditorRectTransformAnchorGizmo::AnchorPosition::BotRight,
                attachedGameObject);

    m_leftTopCorner->SetParent(this);
    m_rightTopCorner->SetParent(this);
    m_leftBotCorner->SetParent(this);
    m_rightBotCorner->SetParent(this);
    m_leftTopAnchor->SetParent(this);
    m_rightTopAnchor->SetParent(this);
    m_leftBotAnchor->SetParent(this);
    m_rightBotAnchor->SetParent(this);
    m_centerCorner->SetParent(this);
}

EditorRectTransformGizmo::~EditorRectTransformGizmo()
{
}

void EditorRectTransformGizmo::OnUpdate()
{
}

void EditorRectTransformGizmo::OnDrawGizmos(bool depthed, bool overlay)
{
    RectTransform *rtrans = m_attachedGameObject->GetComponent<RectTransform>();
    ASSERT(rtrans);
    ASSERT(EditorState::GetCurrentTransformMode() ==
           EditorState::RectTransform);

    if (overlay)
    {
        Rect rect = rtrans->GetScreenSpaceRect(true);
        Gizmos::SetColor(Color::White);
        Gizmos::SetLineWidth(2.0f);
        Gizmos::RenderRect(rect);
    }
}

bool EditorRectTransformGizmo::SomeOtherCornerGrabbed(
        EditorRectTransformCornerGizmo *corner) const
{
    return
       (corner == m_leftTopCorner  ? false : m_leftTopCorner->IsGrabbed())  ||
       (corner == m_rightTopCorner ? false : m_rightTopCorner->IsGrabbed()) ||
       (corner == m_leftBotCorner  ? false : m_leftBotCorner->IsGrabbed())  ||
       (corner == m_rightBotCorner ? false : m_rightBotCorner->IsGrabbed()) ||
       (corner == m_centerCorner   ? false : m_centerCorner->IsGrabbed());
}

