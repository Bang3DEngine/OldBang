#ifndef RECTTRANSFORM_H
#define RECTTRANSFORM_H

#include "Bang/Vector2.h"
#include "Bang/Transform.h"

class RectTransform : public Transform
{
    COMPONENT(RectTransform)

public:
    RectTransform();
    virtual ~RectTransform();

    void SetMarginLeft(int marginLeft);
    void SetMarginTop(int marginTop);
    void SetMarginRight(int marginRight);
    void SetMarginBot(int marginBot);
    void SetMargins(int marginAll);
    void SetMargins(const Vector2i& marginRightTop,
                    const Vector2i& marginLeftBot);
    void SetMargins(int left, int top, int right, int bot);
    void SetPivotPosition(const Vector2 &pivotPosition);
    void SetAnchorMin(const Vector2 &anchorMin);
    void SetAnchorMax(const Vector2 &anchorMax);
    void SetAnchors(const Vector2 &anchorMin, const Vector2 &anchorMax);

    int GetMarginLeft()  const;
    int GetMarginTop()   const;
    int GetMarginRight() const;
    int GetMarginBot()   const;
    const Vector2i& GetMarginRightTop() const;
    const Vector2i& GetMarginLeftBot() const;
    const Vector2& GetPivotPosition()  const;
    const Vector2& GetAnchorMin()  const;
    const Vector2& GetAnchorMax() const;
    Recti GetScreenSpaceRectPx() const;
    Rect GetScreenSpaceRectNDC() const;
    Recti GetParentScreenRectPx() const;
    Rect GetParentScreenRect() const;
    virtual const Matrix4& GetLocalToParentMatrix() const override;

    // Conversion functions
    Vector2 FromGlobalNDCToLocalNDC(const Vector2 &globalNDCPoint) const;
    Vector2 FromLocalNDCToGlobalNDC(const Vector2 &localNDCPoint) const;
    Vector2 FromPixelsToLocalNDC(const Vector2i &pixels) const;
    Vector2 FromPixelsAmountToLocalNDC(const Vector2i &pixelsAmount) const;
    static Vector2 FromPixelsAmountToGlobalNDC(const Vector2i &pixelsAmount);
    Vector2i FromLocalNDCToPixelsAmount(const Vector2 &ndcAmount) const;
    static Vector2i FromGlobalNDCToPixelsAmount(const Vector2 &ndcAmount);
    Vector2 FromPixelsPointToLocalNDC(const Vector2i &pixelsPoint) const;
    static Vector2 FromPixelsPointToGlobalNDC(const Vector2 &pixelsPoint);
    static Vector2 FromPixelsPointToGlobalNDC(const Vector2i &pixelsPoint);
    Vector2i FromLocalNDCToPixelsPoint(const Vector2 &ndcPoint) const;
    static Vector2i FromGlobalNDCToPixelsPoint(const Vector2 &ndcPoint);

    // Component
    virtual void OnParentSizeChanged() override;

    // ICloneable
    void CloneInto(ICloneable *clone) const;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    Vector2i m_marginRightTop = Vector2i::Zero;
    Vector2i m_marginLeftBot  = Vector2i::Zero;

    Vector2 m_pivotPosition =  Vector2::Zero;
    Vector2 m_anchorMin     = -Vector2::One;
    Vector2 m_anchorMax     =  Vector2::One;

    void OnChanged();

    friend class Screen;
};

#endif // RECTTRANSFORM_H
