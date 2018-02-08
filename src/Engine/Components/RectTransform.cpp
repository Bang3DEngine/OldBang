#include "Bang/RectTransform.h"

#include "Bang/GL.h"
#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/AARect.h"
#include "Bang/Gizmos.h"
#include "Bang/Window.h"
#include "Bang/XMLNode.h"
#include "Bang/Matrix4.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/UILayoutManager.h"
#include "Bang/ITransformListener.h"

USING_NAMESPACE_BANG

RectTransform::RectTransform()
{
}

RectTransform::~RectTransform()
{
}

Vector2 RectTransform::
FromViewportPointToLocalPointNDC(const Vector2 &vpPoint) const
{
    return (GetLocalToWorldMatrixInv() *
            Vector4(GL::FromViewportPointToViewportPointNDC(vpPoint), 0, 1)).
           xy();
}
Vector2 RectTransform::
FromViewportPointToLocalPointNDC(const Vector2i &vpPoint) const
{
    return FromViewportPointToLocalPointNDC( Vector2(vpPoint) );
}

Vector2 RectTransform::FromViewportAmountToLocalAmountNDC(const Vector2 &vpAmount) const
{
    Rect parentWinRect = GetParentViewportRect();
    Vector2 parentSizePxVp = Vector2::Max(Vector2::One, parentWinRect.GetSize());
    return GL::FromAmountToAmountNDC( Vector2(vpAmount) , parentSizePxVp );
}
Vector2 RectTransform::FromViewportAmountToLocalAmountNDC(const Vector2i &vpAmount) const
{
    return FromWindowAmountToLocalAmountNDC( Vector2(vpAmount) );
}

Vector2 RectTransform::FromWindowAmountToLocalAmountNDC(const Vector2 &winAmount) const
{
    Vector2 winViewportProportion =
       (Vector2(Window::GetActive()->GetSize()) /
        Vector2::Max(Vector2::One, Vector2(GL::GetViewportSize())) );
    Vector2 vpAmount = winAmount * winViewportProportion;
    return FromViewportAmountToLocalAmountNDC(vpAmount);
}
Vector2 RectTransform::
FromWindowAmountToLocalAmountNDC(const Vector2i &winAmount) const
{
    return FromWindowAmountToLocalAmountNDC( Vector2(winAmount) );
}

Vector2 RectTransform::
FromLocalAmountNDCToViewportAmount(const Vector2 &localAmountNDC) const
{
    Rect parentWinRect = GetParentViewportRect();
    Vector2 parentSizePx = Vector2::Max(Vector2::One, parentWinRect.GetSize());
    return GL::FromAmountNDCToAmount(localAmountNDC, parentSizePx);
}

Vector2 RectTransform::
FromLocalPointNDCToViewportPoint(const Vector2 &localPointNDC) const
{
    return GL::FromViewportPointNDCToViewportPoint(
                FromLocalPointNDCToViewportPointNDC(localPointNDC) );
}

Vector2 RectTransform::
FromViewportPointNDCToLocalPointNDC(const Vector2 &vpPointNDC) const
{
    return FromWorldToLocalPoint( Vector3(vpPointNDC, 0) ).xy();
}

AARect RectTransform::
FromViewportAARectNDCToLocalAARectNDC(const AARect &vpAARectNDC) const
{
    return AARect( FromViewportPointNDCToLocalPointNDC(vpAARectNDC.GetMin()),
                   FromViewportPointNDCToLocalPointNDC(vpAARectNDC.GetMax()));

}
AARect RectTransform::
FromLocalAARectNDCToViewportAARectNDC(const AARect &localAARectNDC) const
{
    return AARect( FromLocalPointNDCToViewportPointNDC(localAARectNDC.GetMin()),
                   FromLocalPointNDCToViewportPointNDC(localAARectNDC.GetMax()));
}
Rect RectTransform::
FromViewportRectNDCToLocalRectNDC(const Rect &vpRectNDC) const
{
    return GetLocalToWorldMatrixInv() * vpRectNDC;
}
Rect RectTransform::
FromLocalRectNDCToViewportRectNDC(const Rect &localRectNDC) const
{
    return GetLocalToWorldMatrix() * localRectNDC;
}

Vector2 RectTransform::
FromLocalPointNDCToViewportPointNDC(const Vector2 &localPointNDC) const
{
    return FromLocalToWorldPoint( Vector3(localPointNDC, 0) ).xy();
}

void RectTransform::SetMarginLeft(int marginLeft)
{
    if (GetMarginLeft() != marginLeft)
    {
        m_marginLeftBot.x = marginLeft;
        InvalidateTransform();
    }
}

void RectTransform::SetMarginTop(int marginTop)
{
    if (GetMarginTop() != marginTop)
    {
        m_marginRightTop.y = marginTop;
        InvalidateTransform();
    }
}

void RectTransform::SetMarginRight(int marginRight)
{
    if (GetMarginRight() != marginRight)
    {
        m_marginRightTop.x = marginRight;
        InvalidateTransform();
    }
}

void RectTransform::SetMarginBot(int marginBot)
{
    if (GetMarginBot() != marginBot)
    {
        m_marginLeftBot.y = marginBot;
        InvalidateTransform();
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

void RectTransform::SetMarginLeftBot(const Vector2i &marginLeftBot)
{
    SetMarginLeft(marginLeftBot.x);
    SetMarginBot(marginLeftBot.y);
}

void RectTransform::SetMarginRightTop(const Vector2i &marginRightTop)
{
    SetMarginRight(marginRightTop.x);
    SetMarginTop(marginRightTop.y);
}

void RectTransform::SetMargins(const Vector2i &marginLeftBot,
                               const Vector2i &marginRightTop)
{
    SetMarginLeftBot(marginLeftBot);
    SetMarginRightTop(marginRightTop);
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
    SetMarginBot(bot);
    SetMarginTop(top);
    SetMarginLeft(left);
    SetMarginRight(right);
}

void RectTransform::SetPivotPosition(const Vector2 &pivotPosition)
{
    if (m_pivotPosition != pivotPosition)
    {
        m_pivotPosition = pivotPosition;
        InvalidateTransform();
    }
}

void RectTransform::SetAnchorMin(const Vector2 &anchorMin)
{
    if (m_anchorMin != anchorMin)
    {
        m_anchorMin = anchorMin;
        InvalidateTransform();
    }
}

void RectTransform::SetAnchorMax(const Vector2 &anchorMax)
{
    if (m_anchorMax != anchorMax)
    {
        m_anchorMax = anchorMax;
        InvalidateTransform();
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
    SetAnchorMinX(anchorX[0]);
    SetAnchorMaxX(anchorX[1]);
}

void RectTransform::SetAnchorY(const Vector2 &anchorY)
{
    SetAnchorMinY(anchorY[0]);
    SetAnchorMaxY(anchorY[1]);
}

void RectTransform::SetAnchors(const Vector2 &anchorPoint)
{
    SetAnchors(anchorPoint, anchorPoint);
}

void RectTransform::SetAnchors(const Vector2 &anchorMin,
                               const Vector2 &anchorMax)
{
    SetAnchorMin(anchorMin);
    SetAnchorMax(anchorMax);
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

Rect RectTransform::GetViewportRectNDC() const
{
    return GetLocalToWorldMatrix() * Rect::NDCRect;
}
Rect RectTransform::GetViewportRect() const
{
    return GL::FromViewportRectNDCToViewportRect( GetViewportRectNDC() );
}

Rect RectTransform::GetParentViewportRectNDC() const
{
    GameObject *parent = GetGameObject()->GetParent();
    if (!parent || !parent->GetRectTransform()) { return Rect::NDCRect; }
    return parent->GetRectTransform()->GetViewportRectNDC();
}

Rect RectTransform::GetParentViewportRect() const
{
    return GL::FromWindowRectNDCToWindowRect( GetParentViewportRectNDC() );
}

#include "Bang/DebugRenderer.h"
void RectTransform::CalculateLocalToParentMatrix() const
{
    const Vector2 vpSize ( GL::GetViewportSize() );
    const Rect vpRect = Rect(vpSize * 0.5f, Vector2::Right, vpSize.x, vpSize.y);

    constexpr float Eps = 0.00001f;

    Window *win = Window::GetActive();
    Vector2 winSize = Vector2(win->GetSize());

    GameObject *parent = GetGameObject()->GetParent();
    RectTransform *parentRT = parent ? parent->GetRectTransform() : nullptr;
    const Rect parentRect = parentRT ? parentRT->GetViewportRect() : vpRect;
    Vector2 parentVpSize = parentRect.GetSize();
    parentVpSize = Vector2::Max(parentVpSize, Vector2::One);

    // Vector2 marginLeftBotNDC  = FromWindowAmountToLocalAmountNDC(GetMarginLeftBot());
    // Vector2 marginRightTopNDC = FromWindowAmountToLocalAmountNDC(GetMarginRightTop());
    Vector2 marginLeftBotNDC  = (Vector2(GetMarginLeftBot())  / parentVpSize) * 2.0f;
    Vector2 marginRightTopNDC = (Vector2(GetMarginRightTop()) / parentVpSize) * 2.0f;
    Vector2 minMarginedAnchor (GetAnchorMin() + marginLeftBotNDC);
    Vector2 maxMarginedAnchor (GetAnchorMax() - marginRightTopNDC);
    Vector3 anchorScaling ((maxMarginedAnchor - minMarginedAnchor) * 0.5f, 1);
    if (GetMarginLeftBot().x == -11)
    {
        Debug_Log("Rotation: " << parent->GetTransform()->GetLocalEuler());
        Debug_Peek(parentRT->GetViewportRect());
        Debug_Peek(parentVpSize);
        Debug_Peek(marginLeftBotNDC);
        Debug_Peek(marginRightTopNDC);
        Debug_Peek(minMarginedAnchor);
        Debug_Peek(maxMarginedAnchor);
        Debug_Peek(anchorScaling);

        Debug_Log( Vector2::Dot(parentRect.GetAxis(0), parentRect.GetAxis(1)));

        DebugRenderer::RenderLineNDC(parentRect.GetCenter(), parentRect.GetCenter() + parentRect.GetAxis(0),
                                     Color::Green, 1.0f, 2.0f, false);
        DebugRenderer::RenderLineNDC(parentRect.GetCenter(), parentRect.GetCenter() + parentRect.GetAxis(1),
                                     Color::Green, 1.0f, 2.0f, false);
        DebugRenderer::RenderRectNDC(parentRect, Color::Red, 1.0f, 3.0f, false);
        Debug_Log("==============================\n\n");
    }
    Vector3 moveToAnchorCenter( (maxMarginedAnchor + minMarginedAnchor) * 0.5f, 0 );
    // Vector3 moveToAnchorCenter( parentRect.GetCenter(), 0 );

    Matrix4 scaleToAnchorsMat = Matrix4::ScaleMatrix(anchorScaling);
    Matrix4 translateToAnchorCenterMat = Matrix4::TranslateMatrix(moveToAnchorCenter);

    m_rectLocalToParentMatrix = translateToAnchorCenterMat * scaleToAnchorsMat;
    m_rectLocalToParentMatrixInv = m_rectLocalToParentMatrix.Inversed();

    Matrix4f transformRotation = Matrix4f::Identity;
    if (GetLocalRotation() != Quaternion::Identity)
    {
        float ar = Math::Max(float(win->GetWidth()), Eps) /
                   Math::Max(float(win->GetHeight()), Eps);
        Matrix4 aspectRatio    = Matrix4::ScaleMatrix( Vector3(ar, 1, 1) );
        Matrix4 aspectRatioInv = Matrix4::ScaleMatrix( Vector3(1.0f/ar, 1, 1) );

        anchorScaling = Vector3::Max(anchorScaling, Vector3(Eps));
        Matrix4 scaleToAnchorsInvMat = Matrix4::ScaleMatrix(1.0f/anchorScaling);
        Matrix4f translateToPivotMatrix =
                Matrix4f::TranslateMatrix( Vector3f(-GetPivotPosition(), 0) );
        Matrix4f translateToPivotMatrixInv =
                Matrix4f::TranslateMatrix( Vector3f( GetPivotPosition(), 0) );


        transformRotation =
                aspectRatioInv                                    *
                  // translateToPivotMatrixInv                       *
                    // scaleToAnchorsInvMat                          *
                      Matrix4::RotateMatrix( GetLocalRotation() ) *
                    // scaleToAnchorsMat                             *
                  // translateToPivotMatrix                          *
                aspectRatio *
                Matrix4::Identity;
    }

    m_transformLocalToParentMatrix = Matrix4::TranslateMatrix(GetLocalPosition()) *
                                     transformRotation *
                                     Matrix4::ScaleMatrix(GetLocalScale());
    m_transformLocalToParentMatrixInv = m_transformLocalToParentMatrix.Inversed();

    m_localToParentMatrix = m_transformLocalToParentMatrix * m_rectLocalToParentMatrix;
    // m_localToParentMatrix = m_rectLocalToParentMatrix;
    m_localToParentMatrixInv = m_localToParentMatrix.Inversed();

}

void RectTransform::CalculateLocalToWorldMatrix() const
{
    m_rectLocalToWorldMatrix = GetRectLocalToParentMatrix();
    m_transformLocalToWorldMatrix = GetTransformLocalToParentMatrix();

    const GameObject *parent = GetGameObject()->GetParent();
    if (parent)
    {
        if (parent->GetRectTransform())
        {
            const Matrix4 &rmp = parent->GetRectTransform()->GetRectLocalToWorldMatrix();
            m_rectLocalToWorldMatrix = rmp * m_rectLocalToWorldMatrix;

            const Matrix4 &mp = parent->GetRectTransform()->GetTransformLocalToWorldMatrix();
            m_transformLocalToWorldMatrix = mp * m_transformLocalToWorldMatrix;
        }
        else
        {
            const Matrix4 &mp = parent->GetTransform()->GetLocalToWorldMatrix();
            m_rectLocalToWorldMatrix = mp * m_rectLocalToWorldMatrix;
            m_transformLocalToWorldMatrix = mp * m_transformLocalToWorldMatrix;
        }
    }
    m_rectLocalToWorldMatrixInv = m_rectLocalToWorldMatrix.Inversed();
    m_transformLocalToWorldMatrixInv = m_transformLocalToWorldMatrix.Inversed();

    m_localToWorldMatrix = m_transformLocalToWorldMatrix * m_rectLocalToWorldMatrix;
    // m_localToWorldMatrix = m_rectLocalToWorldMatrix;
    m_localToWorldMatrixInv = m_localToWorldMatrix.Inversed();
}

bool RectTransform::IsMouseOver(bool recursive) const
{
    if (!Input::IsMouseInsideWindow()) { return false; }

    if (IsActive() && GetGameObject()->IsActive() &&
        GetViewportRectNDC().Contains( Input::GetMousePositionNDC() ))
    {
        return true;
    }

    if (recursive)
    {
        List<RectTransform*> childrenRTs =
             GetGameObject()->GetComponentsInChildrenOnly<RectTransform>(true);
        for (RectTransform *childRT : childrenRTs)
        {
            if (childRT->IsMouseOver(false)) { return true; }
        }
    }
    return false;
}

const Matrix4 &RectTransform::GetRectLocalToParentMatrix() const
{
    RecalculateParentMatricesIfNeeded();
    RecalculateWorldMatricesIfNeeded();
    return m_rectLocalToParentMatrix;
}

const Matrix4 &RectTransform::GetRectLocalToWorldMatrix() const
{
    RecalculateParentMatricesIfNeeded();
    RecalculateWorldMatricesIfNeeded();
    return m_rectLocalToWorldMatrix;
}

const Matrix4 &RectTransform::GetRectLocalToParentMatrixInv() const
{
    RecalculateParentMatricesIfNeeded();
    RecalculateWorldMatricesIfNeeded();
    return m_rectLocalToParentMatrixInv;
}

const Matrix4 &RectTransform::GetRectLocalToWorldMatrixInv() const
{
    RecalculateParentMatricesIfNeeded();
    RecalculateWorldMatricesIfNeeded();
    return m_rectLocalToWorldMatrixInv;
}

const Matrix4 &RectTransform::GetTransformLocalToParentMatrix() const
{
    RecalculateParentMatricesIfNeeded();
    RecalculateWorldMatricesIfNeeded();
    return m_transformLocalToParentMatrix;
}

const Matrix4 &RectTransform::GetTransformLocalToWorldMatrix() const
{
    RecalculateParentMatricesIfNeeded();
    RecalculateWorldMatricesIfNeeded();
    return m_transformLocalToWorldMatrix;
}

const Matrix4 &RectTransform::GetTransformLocalToParentMatrixInv() const
{
    RecalculateParentMatricesIfNeeded();
    RecalculateWorldMatricesIfNeeded();
    return m_transformLocalToParentMatrixInv;
}

const Matrix4 &RectTransform::GetTransformLocalToWorldMatrixInv() const
{
    RecalculateParentMatricesIfNeeded();
    RecalculateWorldMatricesIfNeeded();
    return m_transformLocalToWorldMatrixInv;
}

void RectTransform::OnRender(RenderPass rp)
{
    Transform::OnRender(rp);
    /*
    if (rp != RenderPass::Overlay) { return; }

    Gizmos::Reset();
    Gizmos::SetThickness(1.0f);

    Rect r = GetViewportRectNDC(); (void)r;

    // Random::SetSeed(GetInstanceId());
    Gizmos::SetColor(Random::GetColorOpaque());
    // Gizmos::RenderFillRect(r);

    Gizmos::SetColor(Color::Green);
    Gizmos::RenderRect(AARect(r));
    Gizmos::SetColor(Color::Yellow);
    Gizmos::RenderScreenLine(r.GetMinXMaxY(), r.GetMaxXMinY());
    Gizmos::SetColor(Color::Yellow);
    Gizmos::RenderScreenLine(r.GetMinXMinY(), r.GetMaxXMaxY());
    float size = GL::FromViewportAmountToViewportAmountNDC(Vector2(2)).x;
    Gizmos::SetColor(Color::Red);
    Gizmos::RenderRect(Rect(r.GetCenter() - Vector2(size),
                            r.GetCenter() + Vector2(size)));

    Gizmos::SetColor(Color::Blue);
    Rect anchorRect = FromLocalNDCToViewportNDC(
                                Rect(GetAnchorMin(), GetAnchorMax()));
    Gizmos::RenderRect(anchorRect);
    */
}

void RectTransform::CloneInto(ICloneable *clone) const
{
    Transform::CloneInto(clone);
    RectTransform *rt = Cast<RectTransform*>(clone);

    rt->SetMargins( GetMarginLeftBot(), GetMarginRightTop() );
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

void RectTransform::OnEnabled()  { IInvalidatableTransformLocal::Invalidate(); }
void RectTransform::OnDisabled() { IInvalidatableTransformLocal::Invalidate(); }
