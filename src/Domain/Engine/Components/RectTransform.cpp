#include "RectTransform.h"

#include "Math.h"
#include "Debug.h"
#include "Screen.h"
#include "XMLNode.h"
#include "Vector4.h"
#include "GameObject.h"

RectTransform::RectTransform()
{
}

RectTransform::~RectTransform()
{
}

void RectTransform::CloneInto(ICloneable *clone) const
{
    Transform::CloneInto(clone);
    RectTransform *rt = static_cast<RectTransform*>(clone);

    rt->SetMarginLeft ( GetMarginLeft()  );
    rt->SetMarginTop  ( GetMarginTop()   );
    rt->SetMarginRight( GetMarginRight() );
    rt->SetMarginBot  ( GetMarginBot()   );

    rt->SetAnchors( GetAnchorMin(), GetAnchorMax() );

    rt->SetPivotPosition( GetPivotPosition() );
}


ICloneable *RectTransform::Clone() const
{
    RectTransform *rt = new RectTransform();
    CloneInto(rt);
    return rt;
}

void RectTransform::SetMarginLeft(int marginLeft)
{
    m_marginLeft = marginLeft;
    OnChanged();
}

void RectTransform::SetMarginTop(int marginTop)
{
    m_marginTop = marginTop;
    OnChanged();
}

void RectTransform::SetMarginRight(int marginRight)
{
    m_marginRight = marginRight;
    OnChanged();
}

void RectTransform::SetMarginBot(int marginBot)
{
    m_marginBot = marginBot;
    OnChanged();
}

void RectTransform::SetMargins(int left, int top, int right, int bot)
{
    SetMarginLeft(left);
    SetMarginTop(top);
    SetMarginRight(right);
    SetMarginBot(bot);
}

void RectTransform::SetPivotPosition(const Vector2 &pivotPosition)
{
    m_pivotPosition = pivotPosition;
    OnChanged();
}

void RectTransform::SetAnchorMin(const Vector2 &anchorMin)
{
    m_anchorMin = anchorMin;
    m_anchorMin.x = Math::Min(m_anchorMin.x, m_anchorMax.x);
    m_anchorMin.y = Math::Min(m_anchorMin.y, m_anchorMax.y);
    OnChanged();
}

void RectTransform::SetAnchorMax(const Vector2 &anchorMax)
{
    m_anchorMax = anchorMax;
    m_anchorMax.x = Math::Max(m_anchorMax.x, m_anchorMin.x);
    m_anchorMax.y = Math::Max(m_anchorMax.y, m_anchorMin.y);
    OnChanged();
}

void RectTransform::SetAnchors(const Vector2 &anchorMin,
                               const Vector2 &anchorMax)
{
    // Bypass Min/Max clamping. Sometimes needed.
    m_anchorMin = anchorMin;
    m_anchorMax = anchorMax;
    SetAnchorMin(anchorMin);
    SetAnchorMax(anchorMax);
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

    const Matrix4 &localToParentMatrix =
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
    List<RectTransform*> rectTransforms =
            gameObject->GetComponentsInChildren<RectTransform>();
    for (RectTransform *rt : rectTransforms)
    {
        rt->OnParentSizeChanged();
    }
}

void RectTransform::OnParentSizeChanged()
{
    OnChanged();
}

const Matrix4 &RectTransform::GetLocalToParentMatrix() const
{
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
    parentSizeInPx.x = Math::Max(1.0f, parentSizeInPx.x);
    parentSizeInPx.y = Math::Max(1.0f, parentSizeInPx.y);

    float marginMultiplier = takeMarginsIntoAccount ? 4.0f : 0.0f;
    Vector2 pixelSizeInParent = (1.0f / parentSizeInPx) * marginMultiplier;

    Vector2 minMarginedAnchor
            (m_anchorMin + GetMarginLeftBot()  * pixelSizeInParent);
    Vector2 maxMarginedAnchor
            (m_anchorMax - GetMarginRightTop() * pixelSizeInParent);
    Vector3 anchorScalingV
            ((maxMarginedAnchor - minMarginedAnchor) * 0.5f, 1);
    Matrix4 anchorScaling = Matrix4::ScaleMatrix(anchorScalingV);

    Vector3 moveToPivotV(-m_pivotPosition, 0);
    Matrix4 moveToPivot = Matrix4::TranslateMatrix(moveToPivotV);

    Vector3 moveToAnchorCenterV(
                (maxMarginedAnchor + minMarginedAnchor) * 0.5f, 0);
    Matrix4 moveToAnchorCenter =
            Matrix4::TranslateMatrix(moveToAnchorCenterV);

    //bool beforeHasChanged = m_hasChanged;
    //Matrix4 rotScaleTransform = Transform::GetLocalToParentMatrix();
    //m_hasChanged = beforeHasChanged;
    return //rotScaleTransform *
           moveToAnchorCenter *
           anchorScaling *
           moveToPivot;
}

void RectTransform::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Transform::ReadXMLInfo(xmlInfo);

    SetMarginLeft ( xmlInfo->GetInt("MarginLeft")  );
    SetMarginTop  ( xmlInfo->GetInt("MarginTop")   );
    SetMarginRight( xmlInfo->GetInt("MarginRight") );
    SetMarginBot  ( xmlInfo->GetInt("MarginBot")   );

    SetPivotPosition( xmlInfo->GetVector2("PivotPosition") );
    SetAnchorMin    ( xmlInfo->GetVector2("AnchorMin")     );
    SetAnchorMax    ( xmlInfo->GetVector2("AnchorMax")     );
}

void RectTransform::FillXMLInfo(XMLNode *xmlInfo) const
{
    xmlInfo->SetInt("MarginLeft",  GetMarginLeft() );
    xmlInfo->SetInt("MarginTop",   GetMarginTop()  );
    xmlInfo->SetInt("MarginRight", GetMarginRight());
    xmlInfo->SetInt("MarginBot",   GetMarginBot()  );

    xmlInfo->SetVector2("PivotPosition",  GetPivotPosition());
    xmlInfo->SetVector2("AnchorMin",      GetAnchorMin()    );
    xmlInfo->SetVector2("AnchorMax",      GetAnchorMax()    );

    Transform::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("RectTransform");
    xmlInfo->SetVector3("Position", Vector3::Zero, {XMLProperty::Hidden});
}
