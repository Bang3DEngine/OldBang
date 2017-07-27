#include "Bang/RectTransform.h"

#include "Bang/Math.h"
#include "Bang/Gizmos.h"
#include "Bang/Screen.h"
#include "Bang/XMLNode.h"
#include "Bang/Vector4.h"
#include "Bang/GameObject.h"

RectTransform::RectTransform()
{
}

RectTransform::~RectTransform()
{
}

void RectTransform::CloneInto(ICloneable *clone) const
{
    Transform::CloneInto(clone);
    RectTransform *rt = SCAST<RectTransform*>(clone);

    rt->SetMargins( GetMarginRightTop(), GetMarginLeftBot() );
    rt->SetAnchors( GetAnchorMin(), GetAnchorMax() );
    rt->SetPivotPosition( GetPivotPosition() );
}

void RectTransform::SetMarginLeft(int marginLeft)
{
    if (GetMarginLeft() != marginLeft)
    {
        m_marginLeftBot.x = marginLeft;
        OnChanged();
    }
}

void RectTransform::SetMarginTop(int marginTop)
{
    if (GetMarginTop() != marginTop)
    {
        m_marginRightTop.y = marginTop;
        OnChanged();
    }
}

void RectTransform::SetMarginRight(int marginRight)
{
    if (GetMarginRight() != marginRight)
    {
        m_marginRightTop.x = marginRight;
        OnChanged();
    }
}

void RectTransform::SetMarginBot(int marginBot)
{
    if (GetMarginBot() != marginBot)
    {
        m_marginLeftBot.y = marginBot;
        OnChanged();
    }
}

void RectTransform::SetMargins(int marginAll)
{
    SetMargins(marginAll, marginAll, marginAll, marginAll);
}

void RectTransform::SetMargins(const Vector2i &marginRightTop,
                               const Vector2i &marginLeftBot)
{
    if (GetMarginRightTop() != marginRightTop ||
        GetMarginLeftBot() != marginLeftBot)
    {
        m_marginRightTop = marginRightTop;
        m_marginLeftBot  = marginLeftBot;
        OnChanged();
    }
}

void RectTransform::SetMargins(int left, int top, int right, int bot)
{
    if (GetMarginLeft()  != left  || GetMarginTop() != top ||
        GetMarginRight() != right || GetMarginBot() != bot)
    {
        m_marginLeftBot.x  = left;
        m_marginRightTop.y = top;
        m_marginRightTop.x = right;
        m_marginLeftBot.y  = bot;
        OnChanged();
    }
}

void RectTransform::SetPivotPosition(const Vector2 &pivotPosition)
{
    if (m_pivotPosition != pivotPosition)
    {
        m_pivotPosition = pivotPosition;
        OnChanged();
    }
}

void RectTransform::SetAnchorMin(const Vector2 &anchorMin)
{
    if (m_anchorMin != anchorMin)
    {
        m_anchorMin = anchorMin;
        OnChanged();
    }
}

void RectTransform::SetAnchorMax(const Vector2 &anchorMax)
{
    if (m_anchorMax != anchorMax)
    {
        m_anchorMax = anchorMax;
        OnChanged();
    }
}

void RectTransform::SetAnchors(const Vector2 &anchorMin,
                               const Vector2 &anchorMax)
{
    if (m_anchorMin != anchorMin || m_anchorMax != anchorMax)
    {
        m_anchorMin = anchorMin;
        m_anchorMax = anchorMax;
        OnChanged();
    }
}

int RectTransform::GetMarginLeft() const
{
    return m_marginLeftBot.x;
}

int RectTransform::GetMarginTop() const
{
    return m_marginRightTop.y;
}

int RectTransform::GetMarginRight() const
{
    return m_marginRightTop.x;
}

int RectTransform::GetMarginBot() const
{
    return m_marginLeftBot.y;
}

const Vector2i& RectTransform::GetMarginLeftBot() const
{
    return m_marginLeftBot;
}

const Vector2i& RectTransform::GetMarginRightTop() const
{
    return m_marginRightTop;
}

const Vector2& RectTransform::GetPivotPosition() const
{
    return m_pivotPosition;
}

const Vector2& RectTransform::GetAnchorMin() const
{
    return m_anchorMin;
}

const Vector2& RectTransform::GetAnchorMax() const
{
    return m_anchorMax;
}

Recti RectTransform::GetScreenSpaceRectPx() const
{
    return Recti( ( GetScreenSpaceRect() * 0.5f + 0.5f) *
                    Vector2f(Screen::GetSize()) );
}

Recti RectTransform::GetParentScreenRectPx() const
{
    return Recti( ( GetParentScreenRect() * 0.5f + 0.5f) *
                  Vector2f(Screen::GetSize()) );
}

Rect RectTransform::GetScreenSpaceRect() const
{
    Matrix4 localToWorld;
    GetLocalToWorldMatrix(&localToWorld);
    return localToWorld * Rect::ScreenRect;
}

Rect RectTransform::GetParentScreenRect() const
{
    Rect parentScreenRect = Rect::ScreenRect;
    if (gameObject->parent)
    {
        RectTransform *parentRectTransform =
                gameObject->parent->GetComponent<RectTransform>();
        if (parentRectTransform)
        {
            parentScreenRect = parentRectTransform->GetScreenSpaceRect();
        }
    }
    return parentScreenRect;
}

void RectTransform::OnChanged()
{
    // Chain the messages.
    // This will trigger the refresh of this RectTransform too
    // (see OnParentSizeChanged in this same class)
    if (gameObject) { gameObject->_OnParentSizeChanged(); }
}

void RectTransform::OnParentSizeChanged()
{
    Transform::OnParentSizeChanged();
    m_hasChanged = true;
    GetLocalToParentMatrix();
}

const Matrix4 &RectTransform::GetLocalToParentMatrix() const
{
    if (!IsEnabled(false)) { return Matrix4::Identity; }
    if (!m_hasChanged) { return m_localToParentMatrix; }

    Vector2i parentSizePx = GetParentScreenRectPx().GetSize();
    parentSizePx = Vector2i::Max(Vector2i::One, parentSizePx);
    Vector2f pixelNDCSize = (1.0f / Vector2f(parentSizePx)) * 2.0f;

    Vector3 moveToPivot(m_pivotPosition, 0);

    Vector2 minMarginedAnchor
            (m_anchorMin + Vector2f(GetMarginLeftBot())  * pixelNDCSize);
    Vector2 maxMarginedAnchor
            (m_anchorMax - Vector2f(GetMarginRightTop()) * pixelNDCSize);
    Vector3 anchorScaling ((maxMarginedAnchor - minMarginedAnchor) * 0.5f, 1);

    Vector3 moveToAnchorCenter(
                (maxMarginedAnchor + minMarginedAnchor) * 0.5f, 0);

    m_localToParentMatrix = Matrix4::TranslateMatrix(moveToAnchorCenter) *
                            Matrix4::ScaleMatrix(anchorScaling) *
                            Matrix4::TranslateMatrix(moveToPivot);
    m_hasChanged = false;
    return m_localToParentMatrix;
}

void RectTransform::OnDrawGizmos(GizmosPassType gizmosPassType)
{
    Transform::OnDrawGizmos(gizmosPassType);

    Vector2 size(0.05f);
    Color col = gameObject->name.Contains("ack") ? Color::Red : Color::Green;

    Vector2 amin = GetAnchorMin(), amax = GetAnchorMax();
    Gizmos::SetColor(col);
    Vector2 p = Vector2(amin.x, amin.y);
    Gizmos::RenderFillRect( Rect(p-size/2.0f, p+size/2.0f) );
    Gizmos::SetColor(col);
    p = Vector2(amin.x, amax.y);
    Gizmos::RenderFillRect( Rect(p-size/2.0f, p+size/2.0f) );
    Gizmos::SetColor(col);
    p = Vector2(amax.x, amax.y);
    Gizmos::RenderFillRect( Rect(p-size/2.0f, p+size/2.0f) );
    Gizmos::SetColor(col);
    p = Vector2(amax.x, amin.y);
    Gizmos::RenderFillRect( Rect(p-size/2.0f, p+size/2.0f) );
    Gizmos::SetColor(col);
    Gizmos::RenderRect( GetScreenSpaceRect() );
}

void RectTransform::Read(const XMLNode &xmlInfo)
{
    Transform::Read(xmlInfo);

    SetMargins (xmlInfo.GetVector2<int>("MarginLeftBot"),
                xmlInfo.GetVector2<int>("MarginRightTop"));
    SetPivotPosition( xmlInfo.GetVector2("PivotPosition") );
    SetAnchorMin    ( xmlInfo.GetVector2("AnchorMin")     );
    SetAnchorMax    ( xmlInfo.GetVector2("AnchorMax")     );
}

void RectTransform::Write(XMLNode *xmlInfo) const
{
    Transform::Write(xmlInfo);

    xmlInfo->SetVector2("MarginLeftBot",  GetMarginLeftBot() );
    xmlInfo->SetVector2("MarginRightTop", GetMarginRightTop());

    xmlInfo->SetVector2("PivotPosition",  GetPivotPosition());
    xmlInfo->SetVector2("AnchorMin",      GetAnchorMin()    );
    xmlInfo->SetVector2("AnchorMax",      GetAnchorMax()    );
}
