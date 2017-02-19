#include "EditorRectTransformCornerGizmo.h"

#include "Math.h"
#include "Debug.h"
#include "Scene.h"
#include "Screen.h"
#include "Texture2D.h"
#include "EditorState.h"
#include "SceneManager.h"
#include "RectTransform.h"
#include "AssetsManager.h"

EditorRectTransformCornerGizmo::EditorRectTransformCornerGizmo(
        CornerPosition cornerPosition,
        GameObject *attachedGameObject) : EditorGizmo(attachedGameObject)
{
    ChangeTransformByRectTransform();
    m_attachedRectTransform = attachedGameObject->GetComponent<RectTransform>();

    m_cursorIconWhenOver    = Cursor::CursorIcon::OpenHand;
    m_cursorIconWhenGrabbed = Cursor::CursorIcon::ClosedHand;

    m_cornerPosition = cornerPosition;
    m_circleTexture =
            AssetsManager::Load<Texture2D>("Textures/CircleIcon.btex2d", true);
}

EditorRectTransformCornerGizmo::~EditorRectTransformCornerGizmo()
{
}

void EditorRectTransformCornerGizmo::OnUpdate()
{
    EditorGizmo::OnUpdate();

    if (!m_mouseIsOver && !m_grabbed)
    {
        m_cornerColor = Color(0.0f, 0.0f, 0.6f);
    }
    else
    {
        m_cornerColor = Color(0.0f, 0.5f, 1.0f);
    }
    m_cornerColor.a = 1.0f;

    if (m_grabbed)
    {
        Vector2 mpLast = Input::GetPreviousMouseCoords() / Screen::GetSize();
        Vector2 mp = Input::GetMouseCoords() / Screen::GetSize();
        Vector2 cornerDisplacement = mp - mpLast;
        ApplyMarginDisplacement(cornerDisplacement);
    }
}

void EditorRectTransformCornerGizmo::OnDrawGizmosOverlay()
{
    RectTransform *rtrans = m_attachedGameObject->GetComponent<RectTransform>();
    ASSERT(rtrans);
    ASSERT(EditorState::GetCurrentTransformMode() ==
           EditorState::RectTransform);

    Rect rect = rtrans->GetScreenSpaceRect(true);
    const Vector2 cornerOffset = GetCornerOffset();
    const Vector2 circleCenter = rect.GetCenter() + cornerOffset;
    const Vector2 circleSize = Vector2(0.025f);

    const float borderColor = (m_mouseIsOver || m_grabbed) ? 1.5f : 1.25f;
    const Rect circleBorderRect(circleCenter - circleSize * borderColor,
                                circleCenter + circleSize * borderColor);
    Gizmos::SetColor(Color::White);
    Gizmos::RenderScreenIcon(m_circleTexture, circleBorderRect);

    const Rect circleRect(circleCenter - circleSize,
                          circleCenter + circleSize);
    Gizmos::SetColor(m_cornerColor);
    Gizmos::RenderScreenIcon(m_circleTexture, circleRect);
}

Vector2 EditorRectTransformCornerGizmo::GetCornerOffset() const
{
    RectTransform *rtrans = m_attachedGameObject->GetComponent<RectTransform>();
    Rect rect = rtrans->GetScreenSpaceRect(true);

    Vector2 off = Vector2::One;
    Vector2 rectSize = rect.GetSize() * 0.5f;
    if (m_cornerPosition == CornerPosition::TopLeft ) { off = Vector2(-1, 1); }
    if (m_cornerPosition == CornerPosition::TopRight) { off = Vector2( 1, 1); }
    if (m_cornerPosition == CornerPosition::BotLeft ) { off = Vector2(-1,-1); }
    if (m_cornerPosition == CornerPosition::BotRight) { off = Vector2( 1,-1); }
    if (m_cornerPosition == CornerPosition::Center)   { off = Vector2( 0, 0); }

    return rectSize * off;
}

void EditorRectTransformCornerGizmo::ApplyMarginDisplacement(
        const Vector2 &mouseDisp)
{
    Vector2 d = mouseDisp * Screen::GetSize();

    int marginLeft  = m_attachedRectTransform->GetMarginLeft();
    int marginTop   = m_attachedRectTransform->GetMarginTop();
    int marginRight = m_attachedRectTransform->GetMarginRight();
    int marginBot   = m_attachedRectTransform->GetMarginBot();

    if (m_cornerPosition == CornerPosition::TopLeft)
    {
        marginLeft += d.x;
        marginTop  += d.y;
    }
    else if (m_cornerPosition == CornerPosition::TopRight)
    {
        marginRight -= d.x;
        marginTop   += d.y;
    }
    else if (m_cornerPosition == CornerPosition::BotLeft)
    {
        marginLeft += d.x;
        marginBot  -= d.y;
    }
    else if (m_cornerPosition == CornerPosition::BotRight)
    {
        marginRight -= d.x;
        marginBot   -= d.y;
    }
    else if (m_cornerPosition == CornerPosition::Center)
    {
        marginLeft  += d.x;
        marginTop   += d.y;
        marginRight -= d.x;
        marginBot   -= d.y;
    }

    m_attachedRectTransform->SetMarginLeft (marginLeft);
    m_attachedRectTransform->SetMarginTop  (marginTop);
    m_attachedRectTransform->SetMarginRight(marginRight);
    m_attachedRectTransform->SetMarginBot  (marginBot);
}

