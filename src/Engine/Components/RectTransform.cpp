#include "Bang/RectTransform.h"

#include "Bang/GL.h"
#include "Bang/Rect.h"
#include "Bang/Gizmos.h"
#include "Bang/XMLNode.h"
#include "Bang/Matrix4.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/UILayoutManager.h"
#include "Bang/IRectTransformListener.h"

USING_NAMESPACE_BANG

RectTransform::RectTransform()
{
}

RectTransform::~RectTransform()
{
}

Vector2 RectTransform::FromPixelsToLocalNDC(const Vector2i &pixels) const
{
    Vector2i parentSizePx = GetParentScreenRectPx().GetSize();
    parentSizePx = Vector2i::Max(Vector2i::One, parentSizePx);
    Vector2f pixelNDCSize = (1.0f / Vector2f(parentSizePx)) * 2.0f;
    return Vector2f(pixels) * pixelNDCSize;
}

Vector2 RectTransform::FromPixelsAmountToLocalNDC(const Vector2i &pixelsAmount) const
{
    return (Vector2f(pixelsAmount) / Vector2f(GetScreenSpaceRectPx().GetSize()))
            * 2.0f;
}

Vector2i RectTransform::FromLocalNDCToPixelsAmount(const Vector2 &ndcAmount) const
{
    Vector2 res (ndcAmount * Vector2f(GetScreenSpaceRectPx().GetSize()) * 0.5f);
    return Vector2i( Vector2::Round(res) );
}

Vector2 RectTransform::FromPixelsPointToLocalNDC(const Vector2 &pixelsPoint) const
{
    return FromGlobalNDCToLocalNDC( GL::FromPixelsPointToGlobalNDC(pixelsPoint) );
}

Vector2 RectTransform::FromPixelsPointToLocalNDC(const Vector2i &pixelsPoint) const
{
    return FromPixelsPointToLocalNDC( Vector2(pixelsPoint) );
}

Vector2i RectTransform::FromLocalNDCToPixelsPoint(const Vector2 &ndcPoint) const
{
    return GL::FromGlobalNDCToPixelsPoint( FromLocalNDCToGlobalNDC(ndcPoint) );
}

Vector2 RectTransform::FromGlobalNDCToLocalNDC(const Vector2 &globalNDCPoint) const
{
    return WorldToLocalPoint( Vector3(globalNDCPoint, 0) ).xy();
}

Vector2 RectTransform::FromLocalNDCToGlobalNDC(const Vector2 &localNDCPoint) const
{
    return LocalToWorldPoint( Vector3(localNDCPoint, 0) ).xy();
}

void RectTransform::SetMarginLeft(int marginLeft)
{
    if (GetMarginLeft() != marginLeft)
    {
        m_marginLeftBot.x = marginLeft;
        Invalidate();
    }
}

void RectTransform::SetMarginTop(int marginTop)
{
    if (GetMarginTop() != marginTop)
    {
        m_marginRightTop.y = marginTop;
        Invalidate();
    }
}

void RectTransform::SetMarginRight(int marginRight)
{
    if (GetMarginRight() != marginRight)
    {
        m_marginRightTop.x = marginRight;
        Invalidate();
    }
}

void RectTransform::SetMarginBot(int marginBot)
{
    if (GetMarginBot() != marginBot)
    {
        m_marginLeftBot.y = marginBot;
        Invalidate();
    }
}

void RectTransform::AddMarginLeft(int marginLeft)
{
    SetMarginLeft(GetMarginLeft() + marginLeft);
}

void RectTransform::AddMarginTop(int marginTop)
{
    SetMarginTop(GetMarginTop() + marginTop);
}

void RectTransform::AddMarginRight(int marginRight)
{
    SetMarginRight(GetMarginRight() + marginRight);
}

void RectTransform::AddMarginBot(int marginBot)
{
    SetMarginBot(GetMarginBot() + marginBot);
}

void RectTransform::SetMargins(int marginAll)
{
    SetMargins(marginAll, marginAll, marginAll, marginAll);
}

void RectTransform::SetMarginMin(Axis axis, int marginMax)
{
    SetMargins(axis, GetMarginMin(axis), marginMax);
}
void RectTransform::SetMarginMax(Axis axis, int marginMin)
{
    SetMargins(axis, marginMin, GetMarginMax(axis));
}

void RectTransform::SetMargins(const Vector2i &marginRightTop,
                               const Vector2i &marginLeftBot)
{
    if (GetMarginRightTop() != marginRightTop ||
        GetMarginLeftBot() != marginLeftBot)
    {
        m_marginRightTop = marginRightTop;
        m_marginLeftBot  = marginLeftBot;
        Invalidate();
    }
}

void RectTransform::SetMargins(Axis axis, const Vector2i &margins)
{
    SetMargins(axis, margins[0], margins[1]);
}

void RectTransform::SetMargins(Axis axis, int marginMin, int marginMax)
{
    if (axis == Axis::Vertical)
    { SetMarginBot(marginMin); SetMarginTop(marginMax); }
    if (axis == Axis::Horizontal)
    { SetMarginLeft(marginMin); SetMarginRight(marginMax); }
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
        Invalidate();
    }
}

void RectTransform::SetPivotPosition(const Vector2 &pivotPosition)
{
    if (m_pivotPosition != pivotPosition)
    {
        m_pivotPosition = pivotPosition;
        Invalidate();
    }
}

void RectTransform::SetAnchorMin(const Vector2 &anchorMin)
{
    if (m_anchorMin != anchorMin)
    {
        m_anchorMin = anchorMin;
        Invalidate();
    }
}

void RectTransform::SetAnchorMax(const Vector2 &anchorMax)
{
    if (m_anchorMax != anchorMax)
    {
        m_anchorMax = anchorMax;
        Invalidate();
    }
}

void RectTransform::SetAnchorMinX(float anchorMinX)
{
    SetAnchorMin( Vector2(anchorMinX, GetAnchorMin().y) );
}

void RectTransform::SetAnchorMinY(float anchorMinY)
{
    SetAnchorMin( Vector2(GetAnchorMin().x, anchorMinY) );
}

void RectTransform::SetAnchorMaxX(float anchorMaxX)
{
    SetAnchorMax( Vector2(anchorMaxX, GetAnchorMax().y) );
}

void RectTransform::SetAnchorMaxY(float anchorMaxY)
{
    SetAnchorMax( Vector2(GetAnchorMax().x, anchorMaxY) );
}

void RectTransform::SetAnchorX(const Vector2 &anchorX)
{
    SetAnchorMinX(anchorX.x);
    SetAnchorMaxX(anchorX.y);
}

void RectTransform::SetAnchorY(const Vector2 &anchorY)
{
    SetAnchorMinY(anchorY.x);
    SetAnchorMaxY(anchorY.y);
}

void RectTransform::SetAnchors(const Vector2 &anchorPoint)
{
    SetAnchors(anchorPoint, anchorPoint);
}

void RectTransform::SetAnchors(const Vector2 &anchorMin,
                               const Vector2 &anchorMax)
{
    if (m_anchorMin != anchorMin || m_anchorMax != anchorMax)
    {
        m_anchorMin = anchorMin;
        m_anchorMax = anchorMax;
        Invalidate();
    }
}

void RectTransform::SetWidthFromPivot(int width)
{
    SetMarginLeft(0); SetMarginRight(0);
    AddWidthFromPivot(width);
}

void RectTransform::SetHeightFromPivot(int height)
{
    SetMarginBot(0); SetMarginTop(0);
    AddHeightFromPivot(height);
}

void RectTransform::AddWidthFromPivot(int width)
{
    SetMarginLeft ( -(GetPivotPosition().x - (-1)) * width / 2 );
    SetMarginRight(  (GetPivotPosition().x - ( 1)) * width / 2 );
}

void RectTransform::AddHeightFromPivot(int height)
{
    SetMarginBot( -(GetPivotPosition().y - (-1)) * height / 2 );
    SetMarginTop(  (GetPivotPosition().y - ( 1)) * height / 2 );
}

int RectTransform::GetMarginLeft() const { return GetMarginLeftBot().x; }
int RectTransform::GetMarginTop() const { return GetMarginRightTop().y; }
int RectTransform::GetMarginRight() const { return GetMarginRightTop().x; }
int RectTransform::GetMarginBot() const { return GetMarginLeftBot().y; }
int RectTransform::GetMarginMin(Axis axis) const { return GetMargins(axis)[0]; }
int RectTransform::GetMarginMax(Axis axis) const { return GetMargins(axis)[1]; }
Vector2 RectTransform::GetMargins(Axis axis) const
{
    return axis == Axis::Horizontal ?
                Vector2(GetMarginLeft(), GetMarginRight()) :
                Vector2(GetMarginBot(),  GetMarginTop());

}

const Vector2i& RectTransform::GetMarginLeftBot() const { return m_marginLeftBot; }
const Vector2i& RectTransform::GetMarginRightTop() const { return m_marginRightTop; }
const Vector2& RectTransform::GetPivotPosition() const { return m_pivotPosition; }
const Vector2& RectTransform::GetAnchorMin() const { return m_anchorMin; }
const Vector2& RectTransform::GetAnchorMax() const { return m_anchorMax; }

Recti RectTransform::GetScreenSpaceRectPx() const
{
    Rect rectNDC = GetScreenSpaceRectNDC();
    return GL::FromGlobalNDCToPixels(rectNDC);
}

Recti RectTransform::GetParentScreenRectPx() const
{
    return Recti( ( GetParentScreenRect() * 0.5f + 0.5f) *
                  Vector2f(GL::GetViewportSize()) );
}

Rect RectTransform::GetScreenSpaceRectNDC() const
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
            parentScreenRect = parentRectTransform->GetScreenSpaceRectNDC();
        }
    }
    return parentScreenRect;
}

const Matrix4 &RectTransform::GetLocalToParentMatrix() const
{
    if (!IsInvalid()) { return m_localToParentMatrix; }

    Vector2d minMarginedAnchor (GetAnchorMin() + FromPixelsToLocalNDC(GetMarginLeftBot()));
    Vector2d maxMarginedAnchor (GetAnchorMax() - FromPixelsToLocalNDC(GetMarginRightTop()));
    Vector3d anchorScaling ((maxMarginedAnchor - minMarginedAnchor) * 0.5, 1);

    Vector3d moveToAnchorCenter( (maxMarginedAnchor + minMarginedAnchor) * 0.5,
                                  0);

    Matrix4d rtMatrix = Matrix4d::TranslateMatrix(moveToAnchorCenter) *
                        Matrix4d::ScaleMatrix(anchorScaling);
    Matrix4d transformMatrix ( Transform::GetLocalToParentMatrix() );

    m_localToParentMatrix = Matrix4f( transformMatrix * rtMatrix );
    Validate();

    return m_localToParentMatrix;
}

Rect RectTransform::FromGlobalNDCToLocalNDC(const Rect &globalNDCRect) const
{
    return Rect( FromGlobalNDCToLocalNDC(globalNDCRect.GetMin()),
                 FromGlobalNDCToLocalNDC(globalNDCRect.GetMax()) );
}

void RectTransform::OnRenderGizmos()
{
    Transform::OnRenderGizmos();
    Gizmos::SetLineWidth(1.0f);

    Rect r = GetScreenSpaceRectNDC();

    // Random::SetSeed(GetInstanceId());
    Gizmos::SetColor(Random::GetColorOpaque());
    // Gizmos::RenderFillRect(r);

    Color c = Color::Green;
    Gizmos::SetColor(c);
    // Gizmos::RenderRect(r);

    /*
    Gizmos::SetColor(Color::Yellow);
    Gizmos::RenderScreenLine(r.GetMinXMaxY(), r.GetMaxXMinY());
    Gizmos::SetColor(Color::Yellow);
    Gizmos::RenderScreenLine(r.GetMinXMinY(), r.GetMaxXMaxY());
    float size = GL::FromPixelsAmountToGlobalNDC(Vector2i(2)).x;
    Gizmos::SetColor(Color::Red);
    Gizmos::RenderRect(Rect(r.GetCenter() - Vector2(size),
                            r.GetCenter() + Vector2(size)));
    */
}

void RectTransform::CloneInto(ICloneable *clone) const
{
    Transform::CloneInto(clone);
    RectTransform *rt = SCAST<RectTransform*>(clone);

    rt->SetMargins( GetMarginRightTop(), GetMarginLeftBot() );
    rt->SetAnchors( GetAnchorMin(), GetAnchorMax() );
    rt->SetPivotPosition( GetPivotPosition() );
}

void RectTransform::ImportXML(const XMLNode &xmlInfo)
{
    Transform::ImportXML(xmlInfo);

    if (xmlInfo.Contains("MarginLeftBot"))
    {
        SetMargins (xmlInfo.Get<Vector2i>("MarginLeftBot"),
                    xmlInfo.Get<Vector2i>("MarginRightTop"));
    }

    if (xmlInfo.Contains("PivotPosition"))
    { SetPivotPosition( xmlInfo.Get<Vector2>("PivotPosition") ); }

    if (xmlInfo.Contains("AnchorMin"))
    { SetAnchorMin( xmlInfo.Get<Vector2>("AnchorMin") ); }

    if (xmlInfo.Contains("AnchorMax"))
    { SetAnchorMax( xmlInfo.Get<Vector2>("AnchorMax") ); }
}

void RectTransform::ExportXML(XMLNode *xmlInfo) const
{
    Transform::ExportXML(xmlInfo);

    xmlInfo->Set("MarginLeftBot",  GetMarginLeftBot() );
    xmlInfo->Set("MarginRightTop", GetMarginRightTop());

    xmlInfo->Set("PivotPosition",  GetPivotPosition());
    xmlInfo->Set("AnchorMin",      GetAnchorMin()    );
    xmlInfo->Set("AnchorMax",      GetAnchorMax()    );
}

void RectTransform::Invalidate()
{
    IInvalidatable<Transform>::Invalidate();
}

void RectTransform::OnInvalidated()
{
    // Propagate to RectTransformListeners
    OnRectTransformChanged();
}

void RectTransform::OnEnabled()  { Invalidate(); }
void RectTransform::OnDisabled() { Invalidate(); }

void RectTransform::OnRectTransformChanged()
{
    auto rtListeners = GetGameObject()->GetComponents<IRectTransformListener>();
    for (IRectTransformListener *rtListener : rtListeners)
    {
        if (rtListener != this) { rtListener->OnRectTransformChanged(); }
    }
    PropagateParentRectTransformChangedEvent();
    PropagateChildrenRectTransformChangedEvent();
}

void RectTransform::PropagateParentRectTransformChangedEvent() const
{
    // Propagate to children
    auto rtChildrenListeners =
    GetGameObject()->GetComponentsInChildrenOnly<IRectTransformListener>(false);
    for (IRectTransformListener *rtListener : rtChildrenListeners)
    {
        rtListener->OnParentRectTransformChanged();
    }
}

void RectTransform::PropagateChildrenRectTransformChangedEvent() const
{
    // Propagate to parent
    auto rtParentLists =
            GetGameObject()->GetComponentsInParent<IRectTransformListener>(false);
    for (IRectTransformListener *parentRTListener : rtParentLists)
    {
        parentRTListener->OnChildrenRectTransformChanged();
    }
}

void RectTransform::OnParentRectTransformChanged()
{
    Invalidate();
}

void RectTransform::OnChildrenRectTransformChanged()
{
    PropagateChildrenRectTransformChangedEvent();
}
