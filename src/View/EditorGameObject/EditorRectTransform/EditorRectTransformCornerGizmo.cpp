#include "EditorRectTransformCornerGizmo.h"

#include "Math.h"
#include "Input.h"
#include "Debug.h"
#include "Scene.h"
#include "Screen.h"
#include "Texture2D.h"
#include "EditorState.h"
#include "SceneManager.h"
#include "RectTransform.h"
#include "AssetsManager.h"
#include "GraphicPipeline.h"
#include "SelectionFramebuffer.h"
#include "EditorRectTransformGizmo.h"

EditorRectTransformCornerGizmo::EditorRectTransformCornerGizmo(
        CornerPosition cornerPosition,
        GameObject *attachedGameObject)
    : EditorGizmo(attachedGameObject)
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
    ASSERT(m_attachedRectTransform);

    m_cornerColor = (!m_mouseIsOver && !m_grabbed) ?
                Color(0.0f, 0.0f, 0.6f) : Color(0.0f, 0.5f, 1.0f);

    EditorRectTransformGizmo *rectTransformGizmo = parent ?
                Object::SCast<EditorRectTransformGizmo>(parent) : nullptr;

    if (m_cornerPosition == CornerPosition::Center)
    {
        // Special case for moving UI. Grab instant (Button instead of ButtonDown)
        // and make sure not grabbing corner and center at the same time
        m_grabbed = m_mouseIsOver &&
                    Input::GetMouseButton(Input::MouseButton::Left) &&
                    !rectTransformGizmo->SomeOtherCornerGrabbed(this);
    }

    if (m_grabbed)
    {
        Vector2 mpLast = Input::GetPreviousMouseCoords() / Screen::GetSize();
        Vector2 mp = Input::GetMouseCoords() / Screen::GetSize();
        Vector2 cornerDisplacement = mp - mpLast;
        ApplyMarginDisplacement(cornerDisplacement);
    }
}

void EditorRectTransformCornerGizmo::OnDrawGizmos(bool depthed, bool overlay)
{
    ASSERT(m_attachedRectTransform);
    ASSERT(EditorState::GetCurrentTransformMode() ==
           EditorState::RectTransform);

    if (overlay)
    {

        Rect rect = m_attachedRectTransform->GetScreenSpaceRect();
        if (GraphicPipeline::GetActive()->GetSelectionFramebuffer()->IsPassing())
        {
            if (m_cornerPosition == CornerPosition::Center)
            {
                // To enable dragging ui from any point, not just the center
                Gizmos::RenderFillRect(rect, Color::Green);
            }
        }

        const Vector2 cornerOffset = GetCornerOffset();
        const Vector2 circleCenter = rect.GetCenter() + cornerOffset;
        const Vector2 circleSize = Vector2(0.025f);

        const float borderColor = (m_mouseIsOver || m_grabbed) ? 1.5f : 1.25f;
        const Rect circleBorderRect(circleCenter - circleSize * borderColor,
                                    circleCenter + circleSize * borderColor);
        Gizmos::SetColor(Color::White);
        Gizmos::RenderScreenIcon(m_circleTexture, circleBorderRect, true);

        const Rect circleRect(circleCenter - circleSize,
                              circleCenter + circleSize);
        Gizmos::SetColor(m_cornerColor);
        Gizmos::RenderScreenIcon(m_circleTexture, circleRect, true);
    }
}

Vector2 EditorRectTransformCornerGizmo::GetCornerOffset() const
{
    RectTransform *rtrans = m_attachedGameObject->GetComponent<RectTransform>();
    Rect rect = rtrans->GetScreenSpaceRect();

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

