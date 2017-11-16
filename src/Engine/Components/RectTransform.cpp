#include "Bang/RectTransform.h"

#include "Bang/GL.h"
#include "Bang/Rect.h"
#include "Bang/Input.h"
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
    Vector2 parentSizePx = GetParentScreenRectPx().GetSize();
    parentSizePx = Vector2::Max(Vector2::One, parentSizePx);
    Vector2f pixelNDCSize = (1.0f / Vector2f(parentSizePx)) * 2.0f;
    return Vector2f(pixels) * pixelNDCSize;
}

Vector2 RectTransform::FromPixelsAmountToLocalNDC(const Vector2i &pixelsAmount) const
{
    return (Vector2f(pixelsAmount) / Vector2f(GetScreenSpaceRectPx().GetSize()))
            * 2.0f;
}

Vector2 RectTransform::FromLocalNDCToPixelsAmount(const Vector2 &ndcAmount) const
{
    return (ndcAmount * Vector2f(GetScreenSpaceRectPx().GetSize()) * 0.5f);
}

Vector2 RectTransform::FromPixelsPointToLocalNDC(const Vector2 &pixelsPoint) const
{
    return FromGlobalNDCToLocalNDC( GL::FromPixelsPointToGlobalNDC(pixelsPoint) );
}

Vector2 RectTransform::FromPixelsPointToLocalNDC(const Vector2i &pixelsPoint) const
{
    return FromPixelsPointToLocalNDC( Vector2(pixelsPoint) );
}

Vector2 RectTransform::FromLocalNDCToPixelsPoint(const Vector2 &ndcPoint) const
{
    return GL::FromGlobalNDCToPixelsPoint( FromLocalNDCToGlobalNDC(ndcPoint) );
}

bool RectTransform::IsMouseOver(bool recursive) const
{
    if (!Input::IsMouseInsideScreen()) { return false; }

    if (GetScreenSpaceRectNDC().Contains( Input::GetMouseCoordsNDC() ))
    { return true; }

    if (recursive)
    {
        List<RectTransform*> childrenRTs =
             GetGameObject()->GetComponentsInChildrenOnly<RectTransform>(false);
        for (RectTransform *childRT : childrenRTs)
        {
            if (childRT->IsMouseOver(true)) { return true; }
        }
    }
    return false;
}

Vector2 RectTransform::FromGlobalNDCToLocalNDC(const Vector2 &globalNDCPoint) const
{
    return WorldToLocalPoint( Vector3(globalNDCPoint, 0) ).xy();
}

Rect RectTransform::FromLocalNDCToGlobalNDC(const Rect &localNDCRect) const
{
    return Rect( FromLocalNDCToGlobalNDC(localNDCRect.GetMin()),
                 FromLocalNDCToGlobalNDC(localNDCRect.GetMax()));
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
    SetMarginLeft ( -(GetPivotPosition().x - (-1)) * width / 2 );
    SetMarginRight(  (GetPivotPosition().x - ( 1)) * width / 2 );
}

void RectTransform::SetHeightFromPivot(int height)
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

Rect RectTransform::GetScreenSpaceRectPx() const
{
    return GL::FromGlobalNDCToPixels( GetScreenSpaceRectNDC() );
}

Rect RectTransform::GetParentScreenRectPx() const
{
    return (GetParentScreenRect() * 0.5f + 0.5f) * Vector2(GL::GetViewportSize());
}

Rect RectTransform::GetScreenSpaceRectNDC() const
{
    return GetLocalToWorldMatrix() * Rect::ScreenRectNDC;
}

Rect RectTransform::GetParentScreenRect() const
{
    Rect parentScreenRect = Rect::ScreenRectNDC;
    if (GetGameObject()->GetParent())
    {
        RectTransform *parentRectTransform =
                GetGameObject()->GetParent()->GetComponent<RectTransform>();
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

    Rect r = GetScreenSpaceRectNDC(); (void)r;

    // Random::SetSeed(GetInstanceId());
    Gizmos::SetColor(Random::GetColorOpaque());
    // Gizmos::RenderFillRect(r);

    Gizmos::SetColor(Color::Green);
    Gizmos::RenderRect(r);
    /*

    Gizmos::SetColor(Color::Yellow);
    Gizmos::RenderScreenLine(r.GetMinXMaxY(), r.GetMaxXMinY());
    Gizmos::SetColor(Color::Yellow);
    Gizmos::RenderScreenLine(r.GetMinXMinY(), r.GetMaxXMaxY());
    float size = GL::FromPixelsAmountToGlobalNDC(Vector2i(2)).x;
    Gizmos::SetColor(Color::Red);
    Gizmos::RenderRect(Rect(r.GetCenter() - Vector2(size),
                            r.GetCenter() + Vector2(size)));

    Gizmos::SetColor(Color::Blue);
    Rect anchorRect = FromLocalNDCToGlobalNDC(
                                Rect(GetAnchorMin(), GetAnchorMax()));
    Gizmos::RenderRect(anchorRect);
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
    List<IRectTransformListener*> propagateTo =
            GetGameObject()->GetComponents<IRectTransformListener>();

    IRectTransformListener::SetReceiveEvents(false);
    PROPAGATE_1(IRectTransformListener, OnRectTransformChanged, propagateTo);
    IRectTransformListener::SetReceiveEvents(true);

    PropagateParentRectTransformChangedEvent();
    PropagateChildrenRectTransformChangedEvent();
}

void RectTransform::PropagateParentRectTransformChangedEvent() const
{
    GameObject *go = GetGameObject();
    PROPAGATE_1(IRectTransformListener, OnParentRectTransformChanged,
                go->GetComponentsInChildrenOnly<IRectTransformListener>(false));
}

void RectTransform::PropagateChildrenRectTransformChangedEvent() const
{
    GameObject *go = GetGameObject();
    PROPAGATE_1(IRectTransformListener, OnChildrenRectTransformChanged,
                go->GetComponentsInParent<IRectTransformListener>(false));
}

void RectTransform::OnParentRectTransformChanged()
{
    Invalidate();
}

void RectTransform::OnChildrenRectTransformChanged()
{
    PropagateChildrenRectTransformChangedEvent();
}
