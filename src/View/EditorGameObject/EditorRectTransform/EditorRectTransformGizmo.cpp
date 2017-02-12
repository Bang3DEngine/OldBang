#include "EditorRectTransformGizmo.h"

#include "Debug.h"
#include "EditorState.h"
#include "RectTransform.h"
#include "EditorRectTransformCornerGizmo.h"

EditorRectTransformGizmo::EditorRectTransformGizmo(
        GameObject *attachedGameObject)
{
    m_attachedGameObject = attachedGameObject;
    m_leftTopCorner = new EditorRectTransformCornerGizmo(
                attachedGameObject,
                EditorRectTransformCornerGizmo::CornerPosition::TopLeft);
    m_leftTopCorner->SetParent(this);

    m_rightTopCorner = new EditorRectTransformCornerGizmo(
                attachedGameObject,
                EditorRectTransformCornerGizmo::CornerPosition::TopRight);
    m_rightTopCorner->SetParent(this);

    m_leftBotCorner = new EditorRectTransformCornerGizmo(
                attachedGameObject,
                EditorRectTransformCornerGizmo::CornerPosition::BotLeft);
    m_leftBotCorner->SetParent(this);

    m_rightBotCorner = new EditorRectTransformCornerGizmo(
                attachedGameObject,
                EditorRectTransformCornerGizmo::CornerPosition::BotRight);
    m_rightBotCorner->SetParent(this);
}

EditorRectTransformGizmo::~EditorRectTransformGizmo()
{
}

void EditorRectTransformGizmo::OnUpdate()
{
}

void EditorRectTransformGizmo::OnDrawGizmos()
{
    RectTransform *rtrans = m_attachedGameObject->GetComponent<RectTransform>();
    ASSERT(rtrans);
    ASSERT(EditorState::GetCurrentTransformMode() ==
           EditorState::RectTransform);

    Rect rect = rtrans->GetScreenContainingRect();
    Gizmos::SetColor(Color::White);
    Gizmos::SetLineWidth(2.0f);
    Gizmos::RenderRect(rect);
}

