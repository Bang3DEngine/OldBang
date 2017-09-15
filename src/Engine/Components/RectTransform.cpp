#include "Bang/RectTransform.h"

#include "Bang/GL.h"
#include "Bang/Rect.h"
#include "Bang/XMLNode.h"
#include "Bang/Matrix4.h"
#include "Bang/GameObject.h"

USING_NAMESPACE_BANG

RectTransform::RectTransform()
{
}

RectTransform::~RectTransform()
{
}

Vector2 RectTransform::FromPixelsAmountToGlobalNDC(const Vector2i &pixels)
{
    return (Vector2f(pixels) / Vector2f(GL::GetViewportSize())) * 2.0f;
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

Vector2i RectTransform::FromGlobalNDCToPixelsAmount(const Vector2 &ndcAmount)
{
    return Vector2i(ndcAmount * Vector2f(GL::GetViewportSize()) * 0.5f);
}
Vector2i RectTransform::FromLocalNDCToPixelsAmount(const Vector2 &ndcAmount) const
{
    return Vector2i(ndcAmount * Vector2f(GetScreenSpaceRectPx().GetSize())
                    * 0.5f);
}

Vector2 RectTransform::FromPixelsPointToLocalNDC(const Vector2i &pixelsPoint) const
{
    return FromGlobalNDCToLocalNDC( FromPixelsPointToGlobalNDC(pixelsPoint) );
}
Vector2 RectTransform::FromPixelsPointToGlobalNDC(const Vector2 &_pixelsPoint)
{
    Vector2i screenSize( GL::GetViewportSize() );
    Vector2i pixelsPoint(_pixelsPoint.x, screenSize.y - _pixelsPoint.y);
    Vector2 res = ( Vector2f(pixelsPoint) / Vector2f(screenSize) ) * 2.0f - 1.0f;
    return res;
}
Vector2 RectTransform::FromPixelsPointToGlobalNDC(const Vector2i &pixelsPoint)
{
    return RectTransform::FromPixelsPointToGlobalNDC( Vector2f(pixelsPoint) );
}

Vector2i RectTransform::FromLocalNDCToPixelsPoint(const Vector2 &ndcPoint) const
{
    return FromGlobalNDCToPixelsPoint( FromLocalNDCToGlobalNDC(ndcPoint) );
}
Vector2i RectTransform::FromGlobalNDCToPixelsPoint(const Vector2 &_ndcPoint)
{
    Vector2 ndcPoint(_ndcPoint.x,  _ndcPoint.y);
    Vector2 screenSize( GL::GetViewportSize() );
    Vector2i resultPx( Vector2::Round( (ndcPoint * 0.5f + 0.5f) * screenSize) );
    return Vector2i(resultPx.x, screenSize.y - resultPx.y);
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
        OnChanged();
    }
}

int RectTransform::GetMarginLeft() const { return m_marginLeftBot.x; }
int RectTransform::GetMarginTop() const { return m_marginRightTop.y; }
int RectTransform::GetMarginRight() const { return m_marginRightTop.x; }
int RectTransform::GetMarginBot() const { return m_marginLeftBot.y; }
const Vector2i& RectTransform::GetMarginLeftBot() const { return m_marginLeftBot; }
const Vector2i& RectTransform::GetMarginRightTop() const { return m_marginRightTop; }
const Vector2& RectTransform::GetPivotPosition() const { return m_pivotPosition; }
const Vector2& RectTransform::GetAnchorMin() const { return m_anchorMin; }
const Vector2& RectTransform::GetAnchorMax() const { return m_anchorMax; }

Recti RectTransform::GetScreenSpaceRectPx() const
{
    Rect rectNDC = GetScreenSpaceRectNDC();
    return Recti(RectTransform::FromGlobalNDCToPixelsPoint(rectNDC.GetMin()),
                 RectTransform::FromGlobalNDCToPixelsPoint(rectNDC.GetMax()));
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
    if (!IsEnabled()) { return Matrix4::Identity; }
    if (!m_hasChanged) { return m_localToParentMatrix; }

    Vector2 minMarginedAnchor (m_anchorMin + FromPixelsToLocalNDC(GetMarginLeftBot()));
    Vector2 maxMarginedAnchor (m_anchorMax - FromPixelsToLocalNDC(GetMarginRightTop()));
    Vector3 anchorScaling ((maxMarginedAnchor - minMarginedAnchor) * 0.5f, 1);

    Vector3 moveToAnchorCenter( (maxMarginedAnchor + minMarginedAnchor) * 0.5f,
                                0);

    Vector3 moveToPivot(m_pivotPosition, 0);

    m_localToParentMatrix = Matrix4::TranslateMatrix(moveToAnchorCenter) *
                            Matrix4::ScaleMatrix(anchorScaling) *
                            Matrix4::TranslateMatrix(moveToPivot);

    m_hasChanged = false;
    return m_localToParentMatrix;
}

void RectTransform::OnChanged()
{
    if (gameObject) { gameObject->ParentSizeChanged(); }
    m_hasChanged = true;
}

void RectTransform::OnParentSizeChanged()
{
    Transform::OnParentSizeChanged();
    m_hasChanged = true;
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
