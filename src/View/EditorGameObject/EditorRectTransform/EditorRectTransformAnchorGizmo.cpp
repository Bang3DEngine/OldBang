#include "EditorRectTransformAnchorGizmo.h"

#include "Math.h"
#include "Time.h"
#include "Input.h"
#include "Debug.h"
#include "Screen.h"
#include "Vector2.h"
#include "Texture2D.h"
#include "EditorState.h"
#include "AssetsManager.h"
#include "RectTransform.h"

EditorRectTransformAnchorGizmo::
    EditorRectTransformAnchorGizmo(AnchorPosition anchorPosition,
                                   GameObject *attachedGameObject)
    : EditorGizmo(attachedGameObject)
{
    ChangeTransformByRectTransform();
    m_attachedRectTransform = attachedGameObject->GetComponent<RectTransform>();

    m_cursorIconWhenOver    = Cursor::CursorIcon::OpenHand;
    m_cursorIconWhenGrabbed = Cursor::CursorIcon::ClosedHand;

    m_anchorPosition = anchorPosition;
    m_anchorTexture =
        AssetsManager::Load<Texture2D>("Textures/RectTransformAnchor.btex2d",
                                       true);
    m_anchorTexture->SetAlphaCutoff(0.1f);
}

EditorRectTransformAnchorGizmo::~EditorRectTransformAnchorGizmo()
{

}

void EditorRectTransformAnchorGizmo::OnUpdate()
{
    EditorGizmo::OnUpdate();

    float fading = (m_mouseIsOver || m_grabbed ? 1.0f : 0.75f);
    m_anchorColor = Color(Color::White * fading, 1);

    if (m_grabbed)
    {
        Vector2 mpLast = Input::GetPreviousMouseCoords() / Screen::GetSize();
        Vector2 mp = Input::GetMouseCoords() / Screen::GetSize();
        Vector2 anchorDisplacement = mp - mpLast;
        ApplyDisplacementToAnchor(anchorDisplacement);
    }
}

void EditorRectTransformAnchorGizmo::OnDrawGizmos(bool depthed, bool overlay)
{
    ASSERT(EditorState::GetCurrentTransformMode() == EditorState::RectTransform);
    if (overlay)
    {
        Gizmos::SetColor( m_anchorColor );
        Gizmos::SetRotation( GetAnchorRotation() );

        const Vector2 anchorCenter = GetAnchorCenter();
        const Rect anchorRect(anchorCenter - c_anchorSize,
                              anchorCenter + c_anchorSize);

        Gizmos::RenderScreenIcon(m_anchorTexture, anchorRect, true);
    }
}

Vector2 EditorRectTransformAnchorGizmo::GetAnchorCenter() const
{
    RectTransform *rtrans = m_attachedGameObject->GetComponent<RectTransform>();

    Vector2 aMin = rtrans->GetAnchorMin();
    Vector2 aMax = rtrans->GetAnchorMax();
    Vector2 anchorPos, offsetSign;
    if (m_anchorPosition == AnchorPosition::TopRight)
    {
        anchorPos = Vector2(aMax.x, aMax.y);
        offsetSign = Vector2(1, 1);
    }
    else if (m_anchorPosition == AnchorPosition::TopLeft )
    {
        anchorPos = Vector2(aMin.x, aMax.y);
        offsetSign = Vector2(-1, 1);
    }
    else if (m_anchorPosition == AnchorPosition::BotLeft )
    {
        anchorPos = Vector2(aMin.x, aMin.y);
        offsetSign = Vector2(-1, -1);
    }
    else if (m_anchorPosition == AnchorPosition::BotRight)
    {
        anchorPos = Vector2(aMax.x, aMin.y);
        offsetSign = Vector2(1, -1);
    }

    Rect screenAnchorRect = rtrans->GetParentScreenRect(true);
    anchorPos.x = Math::Map(anchorPos.x, -1.0f, 1.0f,
                            screenAnchorRect.GetMin().x,
                            screenAnchorRect.GetMax().x);
    anchorPos.y = Math::Map(anchorPos.y, -1.0f, 1.0f,
                            screenAnchorRect.GetMin().y,
                            screenAnchorRect.GetMax().y);

    Vector2 offset = offsetSign * Screen::GetPixelClipSize() * 20.0f;
    return anchorPos + offset;
}

Quaternion EditorRectTransformAnchorGizmo::GetAnchorRotation() const
{
    float angle = 0.0f;
    if (m_anchorPosition == AnchorPosition::TopRight) { angle =  45.0f;  }
    if (m_anchorPosition == AnchorPosition::TopLeft ) { angle = -45.0f;  }
    if (m_anchorPosition == AnchorPosition::BotLeft ) { angle = -135.0f; }
    if (m_anchorPosition == AnchorPosition::BotRight) { angle =  135.0f; }

    return Quaternion::AngleAxis(Math::Deg2Rad(angle), Vector3::Forward);
}

void EditorRectTransformAnchorGizmo::ApplyDisplacementToAnchor(
        const Vector2 &mouseDisp)
{
    Rect parentScreenRect = m_attachedRectTransform->GetParentScreenRect(true);
    Vector2 parentScreenSize( parentScreenRect.GetSize() );
    parentScreenSize.x = Math::Max(1.0f, parentScreenSize.x);
    parentScreenSize.y = Math::Max(1.0f, parentScreenSize.y);
    Vector2 d = mouseDisp / parentScreenSize;
    d *= 2.0f;

    Vector2 anchorMin = m_attachedRectTransform->GetAnchorMin();
    Vector2 anchorMax = m_attachedRectTransform->GetAnchorMax();
    if (m_anchorPosition == AnchorPosition::TopLeft)
    {
        anchorMin += Vector2(d.x, 0.0);
        anchorMax += Vector2(0.0, -d.y);
    }
    else if (m_anchorPosition == AnchorPosition::TopRight)
    {
        anchorMin += Vector2(0.0, 0.0);
        anchorMax += Vector2(d.x, -d.y);
    }
    else if (m_anchorPosition == AnchorPosition::BotLeft)
    {
        anchorMin += Vector2(d.x, -d.y);
        anchorMax += Vector2(0.0, 0.0);
    }
    else if (m_anchorPosition == AnchorPosition::BotRight)
    {
        anchorMin += Vector2(0.0, -d.y);
        anchorMax += Vector2(d.x, 0.0);
    }
    m_attachedRectTransform->SetAnchorMin(anchorMin);
    m_attachedRectTransform->SetAnchorMax(anchorMax);
}

