#ifndef RECTTRANSFORM_H
#define RECTTRANSFORM_H

#include "Matrix4.h"
#include "Vector2.h"
#include "Transform.h"

class XMLNode;
class RectTransform : public Transform
{
/**
  (0,0): left-top.
  (1,1): bot-right.
**/
public:
    virtual ~RectTransform();

    void CloneInto(ICloneable *clone) const;
    ICloneable *Clone() const;

    void SetMarginLeft(int marginLeft);
    void SetMarginTop(int marginTop);
    void SetMarginRight(int marginRight);
    void SetMarginBot(int marginBot);

    void SetPivotPosition(const Vector2 &pivotPosition);
    void SetAnchorPosLeftTop(const Vector2 &anchorPosLeftTop);
    void SetAnchorPosRightBot(const Vector2 &anchorPosRightBot);

    int GetMarginLeft()  const;
    int GetMarginTop()   const;
    int GetMarginRight() const;
    int GetMarginBot()   const;

    const Vector2& GetPivotPosition()  const;
    const Vector2& GetAnchorPosLeftTop()  const;
    const Vector2& GetAnchorPosRightBot() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

protected:
    RectTransform();

private:
    int m_marginLeft  = 0;
    int m_marginTop   = 0;
    int m_marginRight = 0;
    int m_marginBot   = 0;

    Vector2 m_pivotPosition = Vector2::Zero;
    Vector2 m_anchorPosLeftTop  = Vector2::Zero;
    Vector2 m_anchorPosRightBot = Vector2::Zero;

    // This determines the transform the pivot must have applied
    // to move it to the parent coords
    virtual const Matrix4& GetLocalToParentMatrix() const override;

    friend class GameObject;
};

#endif // RECTTRANSFORM_H
