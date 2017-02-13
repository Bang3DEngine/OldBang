#include "EditorRectTransformAnchorGizmo.h"

#include "Math.h"
#include "Time.h"
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
    m_anchorPosition = anchorPosition;
    m_anchorTexture =
        AssetsManager::Load<Texture2D>("Textures/RectTransformAnchor.btex2d",
                                       true);
    m_anchorTexture->SetalphaCutoff(0.1f);
}

EditorRectTransformAnchorGizmo::~EditorRectTransformAnchorGizmo()
{

}

void EditorRectTransformAnchorGizmo::OnUpdate()
{
    float fading = (m_mouseIsOver ? 1.0f : 0.75f);
    m_anchorColor = Color(Color::White * fading, 1);
}

void EditorRectTransformAnchorGizmo::OnDrawGizmosOverlay()
{
    RectTransform *rtrans = m_attachedGameObject->GetComponent<RectTransform>();
    ASSERT(rtrans);
    ASSERT(EditorState::GetCurrentTransformMode() ==
           EditorState::RectTransform);

    const Vector2 anchorCenter = GetAnchorCenter();
    const Vector2 anchorSize = Vector2(c_anchorSize);
    const Rect anchorRect(anchorCenter - anchorSize,
                          anchorCenter + anchorSize);

    Gizmos::SetColor( m_anchorColor );
    Gizmos::SetRotation( GetAnchorRotation() );
    Gizmos::RenderScreenIcon(m_anchorTexture, anchorRect);
}

Vector2 EditorRectTransformAnchorGizmo::GetAnchorCenter() const
{
    RectTransform *rtrans = m_attachedGameObject->GetComponent<RectTransform>();

    Vector2 aMin = rtrans->GetAnchorMin();
    Vector2 aMax = rtrans->GetAnchorMax();
    Vector2 anchorPos, offsetSign;
    if (m_anchorPosition == AnchorPosition::TopRight)
    {
        anchorPos = Vector2(aMax.x, aMin.y);
        offsetSign = Vector2( 1.0f, 1.0f);
    }
    else if (m_anchorPosition == AnchorPosition::TopLeft )
    {
        anchorPos = Vector2(aMin.x, aMin.y);
        offsetSign = Vector2(-1.0f, 1.0f);
    }
    else if (m_anchorPosition == AnchorPosition::BotLeft )
    {
        anchorPos = Vector2(aMin.x, aMax.y);
        offsetSign = Vector2(-1.0f, -1.0f);
    }
    else if (m_anchorPosition == AnchorPosition::BotRight)
    {
        anchorPos = Vector2(aMax.x, aMax.y);
        offsetSign = Vector2(1.0f, -1.0f);
    }

    Rect parentRect = rtrans->GetParentScreenRect();
    anchorPos.x = Math::Map(anchorPos.x, -1.0f, 1.0f,
                            parentRect.GetMin().x, parentRect.GetMax().x);
    anchorPos.y = Math::Map(anchorPos.y, -1.0f, 1.0f,
                            parentRect.GetMin().y, parentRect.GetMax().y);
    anchorPos.y *= -1.0f;

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

