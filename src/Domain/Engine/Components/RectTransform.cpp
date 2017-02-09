#include "RectTransform.h"

#include "Debug.h"
#include "Screen.h"
#include "XMLNode.h"

RectTransform::RectTransform()
{
    Debug_Log("RectTransform constructor");
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

    rt->SetAnchorPosLeftTop ( GetAnchorPosLeftTop()  );
    rt->SetAnchorPosRightBot( GetAnchorPosRightBot() );
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

void RectTransform::SetAnchorPosLeftTop(const Vector2 &anchorPosLeftTop)
{
    m_hasChanged = true;
    m_anchorPosLeftTop = anchorPosLeftTop;
}

void RectTransform::SetAnchorPosRightBot(const Vector2 &anchorPosRightBot)
{
    m_hasChanged = true;
    m_anchorPosRightBot = anchorPosRightBot;
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

const Vector2& RectTransform::GetPivotPosition() const
{
    return m_pivotPosition;
}

const Vector2& RectTransform::GetAnchorPosLeftTop() const
{
    return m_anchorPosLeftTop;
}

const Vector2& RectTransform::GetAnchorPosRightBot() const
{
    return m_anchorPosRightBot;
}

void RectTransform::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Transform::ReadXMLInfo(xmlInfo);

    SetMarginLeft ( xmlInfo->GetInt("MarginLeft")  );
    SetMarginTop  ( xmlInfo->GetInt("MarginTop")   );
    SetMarginRight( xmlInfo->GetInt("MarginRight") );
    SetMarginBot  ( xmlInfo->GetInt("MarginBot")   );

    SetPivotPosition( xmlInfo->GetVector2("PivotPosition") );
    SetAnchorPosLeftTop ( xmlInfo->GetVector2("AnchorLeftTop")  );
    SetAnchorPosRightBot( xmlInfo->GetVector2("AnchorRightBot") );
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
    xmlInfo->SetVector2("AnchorLeftTop",  GetAnchorPosLeftTop());
    xmlInfo->SetVector2("AnchorRightBot", GetAnchorPosRightBot());
}

const Matrix4 &RectTransform::GetLocalToParentMatrix() const
{
    const Vector3 screenSize = Vector3(Screen::GetSize(), 1.0f);
    //if (m_hasChanged) // In case screen size changes
    {
        Rect targetScaleRect( GetAnchorPosLeftTop().x  + GetMarginLeft(),
                              GetAnchorPosRightBot().x - GetMarginRight(),
                              GetAnchorPosLeftTop().y  + GetMarginTop(),
                              GetAnchorPosRightBot().y - GetMarginBot());

        Vector3 pivotTranslate( GetMarginLeft(),
                               -GetMarginTop(),
                                0.0f);
        pivotTranslate /= screenSize;

        Matrix4 translateToPivot = Matrix4::TranslateMatrix(
                    Vector3(GetPivotPosition(), 0) / screenSize );
        Matrix4 scaleToTargetRect = Matrix4::ScaleMatrix(
                    Vector3(targetScaleRect.GetWidth(),
                            targetScaleRect.GetHeight(),
                            1)  / screenSize
                    );
        Matrix4 backFromPivot = translateToPivot.Inversed();

        m_localToParentMatrix = backFromPivot *
                                scaleToTargetRect *
                                translateToPivot;
        m_hasChanged = false;
    }
    return m_localToParentMatrix;
}

