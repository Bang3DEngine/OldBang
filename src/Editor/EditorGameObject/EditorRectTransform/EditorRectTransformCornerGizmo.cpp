#include "Bang/EditorRectTransformCornerGizmo.h"

#include "Bang/Math.h"
#include "Bang/Input.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Screen.h"
#include "Bang/Texture2D.h"
#include "Bang/EditorState.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/AssetsManager.h"
#include "Bang/GraphicPipeline.h"
#include "Bang/SelectionFramebuffer.h"
#include "Bang/EditorRectTransformGizmo.h"

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
    Path circleTexPath = EPATH("Textures/CircleIcon.btex2d");
    m_circleTexture = AssetsManager::Load<Texture2D>(circleTexPath);
}

EditorRectTransformCornerGizmo::~EditorRectTransformCornerGizmo()
{
}

void EditorRectTransformCornerGizmo::OnEditorUpdate()
{
    EditorGizmo::OnEditorUpdate();
    ENSURE(m_attachedRectTransform);

    m_cornerColor = (!m_mouseIsOver && !m_grabbed) ?
                Color(0.0f, 0.0f, 0.6f) : Color(0.0f, 0.5f, 1.0f);

    /*
    if (m_cornerPosition == CornerPosition::Center)
    {
        EditorRectTransformGizmo *rectTransformGizmo = parent ?
                    Object::SCast<EditorRectTransformGizmo>(parent) : nullptr;
        // Special case for moving UI. Grab instant (Button instead of ButtonDown)
        // and make sure not grabbing corner and center at the same time
        m_grabbed = m_mouseIsOver &&
                    Input::GetMouseButton(Input::MouseButton::Left) &&
                    !rectTransformGizmo->SomeOtherCornerGrabbed(this);
    }
    */

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
    ENSURE(m_attachedRectTransform);
    ENSURE(EditorState::GetCurrentTransformMode() ==
           EditorState::RectTransform);

    if (overlay)
    {
        Rect rect = m_attachedRectTransform->GetScreenSpaceRect();
        if (GraphicPipeline::GetActive()->GetSelectionFramebuffer()->IsPassing())
        {
            if (m_cornerPosition == CornerPosition::Center)
            {
                // To enable dragging ui from any point, not just the center
                // Gizmos::RenderFillRect(rect, Color::Green);
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
    d.x = int( Math::Round(d.x) ); // Needed to avoid precission issues
    d.y = int( Math::Round(d.y) );

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

