#include "RectTransform.h"

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
    m_hasChanged = true;
    m_marginLeft = marginLeft;
}

void RectTransform::SetMarginTop(int marginTop)
{
    m_hasChanged = true;
    m_marginTop = marginTop;
}

void RectTransform::SetMarginRight(int marginRight)
{
    m_hasChanged = true;
    m_marginRight = marginRight;
}

void RectTransform::SetMarginBot(int marginBot)
{
    m_hasChanged = true;
    m_marginBot = marginBot;
}

void RectTransform::SetPivotPosition(const Vector2 &pivotPosition)
{
    m_hasChanged = true;
    m_pivotPosition = pivotPosition;
}

void RectTransform::SetAnchorMin(const Vector2 &anchorMin)
{
    m_hasChanged = true;
    m_anchorMin = anchorMin;
}

void RectTransform::SetAnchorMax(const Vector2 &anchorMax)
{
    m_hasChanged = true;
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
    SetAnchorMin ( xmlInfo->GetVector2("AnchorMin")  );
    SetAnchorMax( xmlInfo->GetVector2("AnchorMax") );
}

void RectTransform::FillXMLInfo(XMLNode *xmlInfo) const
{
    Transform::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("RectTransform");

    xmlInfo->SetInt("MarginLeft",  GetMarginLeft());
    xmlInfo->SetInt("MarginTop",   GetMarginTop());
    xmlInfo->SetInt("MarginRight", GetMarginRight());
    xmlInfo->SetInt("MarginBot",   GetMarginBot());

    xmlInfo->SetVector2("PivotPosition",  GetPivotPosition());
    xmlInfo->SetVector2("AnchorMin",  GetAnchorMin());
    xmlInfo->SetVector2("AnchorMax", GetAnchorMax());
}

void RectTransform::OnDrawGizmos()
{
    if (gameObject->IsSelected())
    {
        Vector2 screenSize = Screen::GetSize();

        Rect anchorRect(m_anchorMin, m_anchorMax);
        //anchorRect /= screenSize;
        Gizmos::SetColor(Color::Green);
        Gizmos::RenderRect(anchorRect);

        Rect innerRect(m_anchorMin + GetMarginLeftTop() / screenSize,
                       m_anchorMax - GetMarginRightBot() / screenSize);
        //innerRect /= screenSize;
        Gizmos::SetColor(Color::Red);
        Gizmos::RenderRect(innerRect);

        //Rect pivotPoint (GetPivotPosition(),
        //                 GetPivotPosition() + Vector2::One * 5 / screenSize);
        //pivotPoint /= screenSize;
        //Gizmos::SetColor(Color::Blue);
        //Gizmos::RenderRect(pivotPoint);
    }
}

const Matrix4 &RectTransform::GetLocalToParentMatrix() const
{
    const Vector3 screenSize = Vector3(Screen::GetSize(), 1.0f);
    //if (m_hasChanged) // In case screen size changes
    {
        Vector2 minMarginedAnchor
                (m_anchorMin + GetMarginLeftTop() / screenSize.xy());
        Vector2 maxMarginedAnchor
                (m_anchorMax - GetMarginRightBot() / screenSize.xy());
        Vector3 anchorScalingV
                ((maxMarginedAnchor - minMarginedAnchor) * 0.5f, 1);
        Matrix4 anchorScaling = Matrix4::ScaleMatrix(anchorScalingV);

        Vector3 moveToPivotV(-m_pivotPosition, 0);
        moveToPivotV = (Vector4(moveToPivotV, 1)).xyz();
        moveToPivotV.y *= -1.0f;
        Matrix4 moveToPivot = Matrix4::TranslateMatrix(moveToPivotV);

        Vector3 moveToAnchorCenterV(
                    (m_anchorMin + m_anchorMax) * 0.5f, 0);
        moveToAnchorCenterV.y *= -1.0f;
        Matrix4 moveToAnchorCenter =
                Matrix4::TranslateMatrix(moveToAnchorCenterV);

        m_localToParentMatrix =
                moveToAnchorCenter *
                anchorScaling *
                moveToPivot
                ;

        m_hasChanged = false;
    }
    return m_localToParentMatrix;
}

