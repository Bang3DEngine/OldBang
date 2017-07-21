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

    rt->SetMarginLeft ( GetMarginLeft()  );
    rt->SetMarginTop  ( GetMarginTop()   );
    rt->SetMarginRight( GetMarginRight() );
    rt->SetMarginBot  ( GetMarginBot()   );

    rt->SetAnchors( GetAnchorMin(), GetAnchorMax() );

    rt->SetPivotPosition( GetPivotPosition() );
}

void RectTransform::SetMarginLeft(int marginLeft)
{
    if (m_marginLeft != marginLeft)
    {
        m_marginLeft = marginLeft;
        OnChanged();
    }
}

void RectTransform::SetMarginTop(int marginTop)
{
    if (m_marginTop != marginTop)
    {
        m_marginTop = marginTop;
        OnChanged();
    }
}

void RectTransform::SetMarginRight(int marginRight)
{
    if (m_marginRight != marginRight)
    {
        m_marginRight = marginRight;
        OnChanged();
    }
}

void RectTransform::SetMarginBot(int marginBot)
{
    if (m_marginBot != marginBot)
    {
        m_marginBot = marginBot;
        OnChanged();
    }
}

void RectTransform::SetMargins(int marginAll)
{
    SetMargins(marginAll, marginAll, marginAll, marginAll);
}

void RectTransform::SetMargins(int left, int top, int right, int bot)
{
    if (m_marginLeft  != left  || m_marginTop != top ||
        m_marginRight != right || m_marginBot != bot)
    {
        m_marginLeft  = left;
        m_marginTop   = top;
        m_marginRight = right;
        m_marginBot   = bot;
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
    return m_marginLeft;
}

int RectTransform::GetMarginTop() const
{
    return m_marginTop;
}

int RectTransform::GetMarginRight() const
{
    return m_marginRight;
}

int RectTransform::GetMarginBot() const
{
    return m_marginBot;
}

Vector2 RectTransform::GetMarginLeftBot() const
{
    return Vector2(m_marginLeft, m_marginBot);
}

Vector2 RectTransform::GetMarginRightTop() const
{
    return Vector2(m_marginRight, m_marginTop);
}

Vector2 RectTransform::GetPivotPosition() const
{
    return m_pivotPosition;
}

Vector2 RectTransform::GetAnchorMin() const
{
    return m_anchorMin;
}

Vector2 RectTransform::GetAnchorMax() const
{
    return m_anchorMax;
}

Rect RectTransform::GetScreenSpaceRect(bool takeMarginsIntoAccount) const
{
    Matrix4 parentToWorld;
    if (gameObject->parent)
    {
        gameObject->parent->transform->GetLocalToWorldMatrix(&parentToWorld);
    }

    Matrix4 localToParentMatrix =
            takeMarginsIntoAccount ? GetLocalToParentMatrix() :
                                     GetLocalToParentMatrix(false);
    return parentToWorld * localToParentMatrix * Rect::ScreenRect;
}

Rect RectTransform::GetParentScreenRect(bool takeMarginsIntoAccount) const
{
    Rect parentScreenRect = Rect::ScreenRect;
    if (gameObject->parent)
    {
        RectTransform *parentRectTransform =
                gameObject->parent->GetComponent<RectTransform>();
        if (parentRectTransform)
        {
            parentScreenRect = parentRectTransform->
                    GetScreenSpaceRect(takeMarginsIntoAccount);
        }
    }
    return parentScreenRect;
}

void RectTransform::OnChanged()
{
    m_hasChanged = true;

    ENSURE(gameObject);
    gameObject->_OnParentSizeChanged();
}

const Matrix4 &RectTransform::GetLocalToParentMatrix() const
{
    if (!IsEnabled(false)) { return Matrix4::Identity; }

    if (m_hasChanged)
    {
        m_localToParentMatrix = GetLocalToParentMatrix(true);
        m_hasChanged = false;
    }
    return m_localToParentMatrix;
}

Matrix4 RectTransform::GetLocalToParentMatrix(
        bool takeMarginsIntoAccount) const
{
    const Vector2 screenSize = Screen::GetSize();
    Vector2 parentSizeInPx = GetParentScreenRect(true).GetSize() * screenSize;
    parentSizeInPx = Vector2::Max(Vector2::One, parentSizeInPx);

    float marginMultiplier = takeMarginsIntoAccount ? 4.0f : 0.0f;
    Vector2 pixelSizeInParent = (1.0f / parentSizeInPx) * marginMultiplier;

    Vector2 minMarginedAnchor
            (m_anchorMin + GetMarginLeftBot()  * pixelSizeInParent);
    Vector2 maxMarginedAnchor
            (m_anchorMax - GetMarginRightTop() * pixelSizeInParent);
    Vector3 anchorScalingV
            ((maxMarginedAnchor - minMarginedAnchor) * 0.5f, 1);
    Matrix4 anchorScaling = Matrix4::ScaleMatrix(anchorScalingV);

    Vector3 moveToPivotV(m_pivotPosition, 0);
    Matrix4 moveToPivot = Matrix4::TranslateMatrix(moveToPivotV);

    Vector3 moveToAnchorCenterV(
                (maxMarginedAnchor + minMarginedAnchor) * 0.5f, 0);
    Matrix4 moveToAnchorCenter = Matrix4::TranslateMatrix(moveToAnchorCenterV);

    //bool beforeHasChanged = m_hasChanged;
    //Matrix4 rotScaleTransform = Transform::GetLocalToParentMatrix();
    //m_hasChanged = beforeHasChanged;
    return //rotScaleTransform *
           moveToAnchorCenter *
           anchorScaling *
           moveToPivot;
}

void RectTransform::OnDrawGizmos(GizmosPassType gizmosPassType)
{
    Transform::OnDrawGizmos(gizmosPassType);

    Gizmos::SetColor(Color::Green);
    Gizmos::RenderRect( GetScreenSpaceRect() );

    Gizmos::SetColor(Color::Red);
    Gizmos::RenderRect( GetScreenSpaceRect(false) );
}

void RectTransform::Read(const XMLNode &xmlInfo)
{
    Transform::Read(xmlInfo);

    SetMarginLeft ( xmlInfo.GetInt("MarginLeft")  );
    SetMarginTop  ( xmlInfo.GetInt("MarginTop")   );
    SetMarginRight( xmlInfo.GetInt("MarginRight") );
    SetMarginBot  ( xmlInfo.GetInt("MarginBot")   );

    SetPivotPosition( xmlInfo.GetVector2("PivotPosition") );
    SetAnchorMin    ( xmlInfo.GetVector2("AnchorMin")     );
    SetAnchorMax    ( xmlInfo.GetVector2("AnchorMax")     );
}

void RectTransform::Write(XMLNode *xmlInfo) const
{
    Transform::Write(xmlInfo);

    xmlInfo->SetInt("MarginLeft",  GetMarginLeft() );
    xmlInfo->SetInt("MarginTop",   GetMarginTop()  );
    xmlInfo->SetInt("MarginRight", GetMarginRight());
    xmlInfo->SetInt("MarginBot",   GetMarginBot()  );

    xmlInfo->SetVector2("PivotPosition",  GetPivotPosition());
    xmlInfo->SetVector2("AnchorMin",      GetAnchorMin()    );
    xmlInfo->SetVector2("AnchorMax",      GetAnchorMax()    );
}
