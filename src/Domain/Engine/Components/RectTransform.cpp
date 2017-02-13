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

    rt->SetAnchorMin( GetAnchorMin()  );
    rt->SetAnchorMax( GetAnchorMax()  );

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
    OnChanged();
    m_marginLeft = marginLeft;
}

void RectTransform::SetMarginTop(int marginTop)
{
    OnChanged();
    m_marginTop = marginTop;
}

void RectTransform::SetMarginRight(int marginRight)
{
    OnChanged();
    m_marginRight = marginRight;
}

void RectTransform::SetMarginBot(int marginBot)
{
    OnChanged();
    m_marginBot = marginBot;
}

void RectTransform::SetPivotPosition(const Vector2 &pivotPosition)
{
    OnChanged();
    m_pivotPosition = pivotPosition;
}

void RectTransform::SetAnchorMin(const Vector2 &anchorMin)
{
    OnChanged();
    m_anchorMin = anchorMin;
}

void RectTransform::SetAnchorMax(const Vector2 &anchorMax)
{
    OnChanged();
    m_anchorMax = anchorMax;
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

Vector2 RectTransform::GetMarginLeftTop() const
{
    return Vector2(m_marginLeft, m_marginTop);
}

Vector2 RectTransform::GetMarginRightBot() const
{
    return Vector2(m_marginRight, m_marginBot);
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

Rect RectTransform::GetParentScreenRect() const
{
    Rect parentScreenRect = Rect::ScreenRect;
    RectTransform *parentRectTransform = nullptr;
    if (gameObject->parent)
    {
        parentRectTransform = gameObject->parent->GetComponent<RectTransform>();
    }

    if (parentRectTransform)
    {
        parentScreenRect = parentRectTransform->GetScreenContainingRect();
    }

    return parentScreenRect;
}

Rect RectTransform::GetScreenContainingRect() const
{
    Matrix4 localToWorld;
    GetLocalToWorldMatrix(&localToWorld);
    return localToWorld * Rect::ScreenRect;
}

Rect RectTransform::GetContainingRectInParentSpace() const
{
    return GetLocalToParentMatrix() * Rect::ScreenRect;
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

Matrix4 RectTransform::GetLocalToParentMatrix(bool takeIntoAccountMargins) const
{
    const Vector2 screenSize = Screen::GetSize();
    Vector2 parentSizeInPx = GetParentScreenRect().GetSize() * screenSize;
    parentSizeInPx.x = Math::Max(1.0f, parentSizeInPx.x);
    parentSizeInPx.y = Math::Max(1.0f, parentSizeInPx.y);

    float marginMultiplier = 4.0f; // Dont know why, havent thought it much
    if (!takeIntoAccountMargins) {  marginMultiplier = 0.0f; }
    Vector2 pixelSizeInParent = (1.0f / parentSizeInPx) * marginMultiplier;

    Vector2 minMarginedAnchor
            (m_anchorMin + GetMarginLeftTop()  * pixelSizeInParent);
    Vector2 maxMarginedAnchor
            (m_anchorMax - GetMarginRightBot() * pixelSizeInParent);
    Vector3 anchorScalingV
            ((maxMarginedAnchor - minMarginedAnchor) * 0.5f, 1);
    Matrix4 anchorScaling = Matrix4::ScaleMatrix(anchorScalingV);

    Vector3 moveToPivotV(-m_pivotPosition, 0);
    moveToPivotV.y *= -1.0f;
    Matrix4 moveToPivot = Matrix4::TranslateMatrix(moveToPivotV);

    Vector3 moveToAnchorCenterV(
                (maxMarginedAnchor + minMarginedAnchor) * 0.5f, 0);
    moveToAnchorCenterV.y *= -1.0f;
    Matrix4 moveToAnchorCenter =
            Matrix4::TranslateMatrix(moveToAnchorCenterV);

    const Matrix4 &rotScaleTransform = Transform::GetLocalToParentMatrix();
    return rotScaleTransform *
           moveToAnchorCenter *
           anchorScaling *
           moveToPivot;
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

Matrix4 RectTransform::GetLocalToWorldMatrixAnchors() const
{
    Matrix4 parentToWorld;
    if (gameObject->parent)
    {
        gameObject->parent->transform->GetLocalToWorldMatrix(&parentToWorld);
    }

    Matrix4 localToParent = GetLocalToParentMatrix(false);
    return parentToWorld * localToParent;
}

