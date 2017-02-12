#include "EditorRectTransformCornerGizmo.h"

#include "Debug.h"
#include "Screen.h"
#include "Texture2D.h"
#include "EditorState.h"
#include "RectTransform.h"
#include "AssetsManager.h"

EditorRectTransformCornerGizmo::EditorRectTransformCornerGizmo(
        GameObject *attachedGameObject,
        CornerPosition cornerPosition)
{
    m_attachedGameObject = attachedGameObject;
    m_cornerPosition = cornerPosition;
    m_circleTexture =
            AssetsManager::Load<Texture2D>("Textures/CircleIcon.btex2d", true);
}

EditorRectTransformCornerGizmo::~EditorRectTransformCornerGizmo()
{

}

void EditorRectTransformCornerGizmo::OnUpdate()
{
    const float fading = (m_mouseIsOver ? 1.0f : 0.7f);
    m_cornerColor = fading * Color(0.3f, 0.3f, 1.0f);
    m_cornerColor.a = 1.0f;
}

void EditorRectTransformCornerGizmo::OnDrawGizmos()
{
    RectTransform *rtrans = m_attachedGameObject->GetComponent<RectTransform>();
    ASSERT(rtrans);
    ASSERT(EditorState::GetCurrentTransformMode() ==
           EditorState::RectTransform);

    Rect rect = rtrans->GetScreenContainingRect();
    const Vector2 cornerOffset = GetCornerOffset();
    const Vector2 circleCenter = rect.GetCenter() + cornerOffset;
    const Vector2 circleSize = Screen::GetPixelClipSize() * 10.0f;

    const Rect circleBorderRect(circleCenter - circleSize * 1.5f,
                                circleCenter + circleSize * 1.5f);
    Gizmos::SetColor(Color::White);
    Gizmos::RenderScreenIcon(m_circleTexture, circleBorderRect);

    const Rect circleRect(circleCenter - circleSize,
                          circleCenter + circleSize);
    Gizmos::SetColor(m_cornerColor);
    Gizmos::RenderScreenIcon(m_circleTexture, circleRect);
}

void EditorRectTransformCornerGizmo::OnMouseExit(bool fromChildren)
{
    EditorGameObject::OnMouseExit(fromChildren);
    m_mouseIsOver = false;
}

void EditorRectTransformCornerGizmo::OnMouseEnter(bool fromChildren)
{
    EditorGameObject::OnMouseEnter(fromChildren);
    m_mouseIsOver = true;
}

Vector2 EditorRectTransformCornerGizmo::GetCornerOffset() const
{
    RectTransform *rtrans = m_attachedGameObject->GetComponent<RectTransform>();
    Rect rect = rtrans->GetScreenContainingRect();

    Vector2 off = Vector2::One;
    Vector2 rectSize = rect.GetSize() * 0.5f;
    if (m_cornerPosition == CornerPosition::TopLeft ) { off = Vector2(-1, 1); }
    if (m_cornerPosition == CornerPosition::TopRight) { off = Vector2( 1, 1); }
    if (m_cornerPosition == CornerPosition::BotLeft ) { off = Vector2(-1,-1); }
    if (m_cornerPosition == CornerPosition::BotRight) { off = Vector2( 1,-1); }

    return rectSize * off;
}

