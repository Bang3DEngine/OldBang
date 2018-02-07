#ifndef RECTTRANSFORM_H
#define RECTTRANSFORM_H

#include "Bang/Rect.h"
#include "Bang/Vector2.h"
#include "Bang/Transform.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IObjectListener.h"

NAMESPACE_BANG_BEGIN

class RectTransform : public Transform
{
    COMPONENT(RectTransform)

public:
    RectTransform();
    virtual ~RectTransform();

    void OnRender(RenderPass rp) override;

    void SetMarginLeft(int marginLeft);
    void SetMarginTop(int marginTop);
    void SetMarginRight(int marginRight);
    void SetMarginBot(int marginBot);
    void AddMarginLeft(int marginLeft);
    void AddMarginTop(int marginTop);
    void AddMarginRight(int marginRight);
    void AddMarginBot(int marginBot);
    void SetMargins(int marginAll);
    void SetMarginMin(Axis axis, int marginMax);
    void SetMarginMax(Axis axis, int marginMin);
    void SetMarginLeftBot(const Vector2i& marginLeftBot);
    void SetMarginRightTop(const Vector2i& marginRightTop);
    void SetMargins(const Vector2i& marginLeftBot,
                    const Vector2i& marginRightTop);
    void SetMargins(Axis axis, const Vector2i &margins);
    void SetMargins(Axis axis, int marginMin, int marginMax);
    void SetMargins(int left, int top, int right, int bot);
    void SetPivotPosition(const Vector2 &pivotPosition);
    void SetAnchorMin(const Vector2 &anchorMin);
    void SetAnchorMax(const Vector2 &anchorMax);
    void SetAnchorMinX(float anchorMinX);
    void SetAnchorMinY(float anchorMinY);
    void SetAnchorMaxX(float anchorMaxX);
    void SetAnchorMaxY(float anchorMaxY);
    void SetAnchorX(const Vector2& anchorX);
    void SetAnchorY(const Vector2& anchorY);
    void SetAnchors(const Vector2 &anchorPoint);
    void SetAnchors(const Vector2 &anchorMin, const Vector2 &anchorMax);

    void SetWidthFromPivot(int width);
    void SetHeightFromPivot(int height);

    int GetMarginLeft()  const;
    int GetMarginTop()   const;
    int GetMarginRight() const;
    int GetMarginBot()   const;
    int GetMarginMin(Axis axis) const;
    int GetMarginMax(Axis axis) const;
    Vector2 GetMargins(Axis axis) const;
    const Vector2i& GetMarginRightTop() const;
    const Vector2i& GetMarginLeftBot() const;
    const Vector2& GetPivotPosition()  const;
    const Vector2& GetAnchorMin()  const;
    const Vector2& GetAnchorMax() const;
    Rect GetViewportRect() const;
    Rect GetViewportRectNDC() const;
    Rect GetParentViewportRect() const;
    Rect GetParentViewportRectNDC() const;
    virtual const Matrix4& GetLocalToParentMatrix() const override;

    // Conversion functions
    AARect  FromViewportAARectNDCToLocalAARectNDC(const AARect &vpAARectNDC) const;
    AARect  FromLocalAARectNDCToViewportAARectNDC(const AARect &localAARectNDC) const;
    Rect    FromViewportRectNDCToLocalRectNDC(const Rect &vpRectNDC) const;
    Rect    FromLocalRectNDCToViewportRectNDC(const Rect &localRectNDC) const;
    Vector2 FromViewportPointNDCToLocalPointNDC(const Vector2 &vpPointNDC) const;
    Vector2 FromLocalPointNDCToViewportPointNDC(const Vector2 &localPointNDC) const;
    Vector2 FromViewportPointToLocalPointNDC(const Vector2  &vpPoint) const;
    Vector2 FromViewportPointToLocalPointNDC(const Vector2i &vpPoint) const;
    Vector2 FromViewportAmountToLocalAmountNDC(const Vector2 &vpAmount) const;
    Vector2 FromViewportAmountToLocalAmountNDC(const Vector2i &vpAmount) const;
    Vector2 FromWindowAmountToLocalAmountNDC(const Vector2 &winAmount) const;
    Vector2 FromWindowAmountToLocalAmountNDC(const Vector2i &winAmount) const;
    Vector2 FromLocalAmountNDCToViewportAmount(const Vector2 &localAmountNDC) const;
    Vector2 FromLocalPointNDCToViewportPoint(const Vector2 &localPointNDC) const;

    bool IsMouseOver(bool recursive = false) const;

    // IObjectListener
    void OnEnabled() override;
    void OnDisabled() override;

    // ICloneable
    void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    Vector2i m_marginRightTop = Vector2i::Zero;
    Vector2i m_marginLeftBot  = Vector2i::Zero;

    Vector2 m_pivotPosition =  Vector2::Zero;
    Vector2 m_anchorMin     = -Vector2::One;
    Vector2 m_anchorMax     =  Vector2::One;

    friend class UILayoutManager;
};

NAMESPACE_BANG_END

#endif // RECTTRANSFORM_H
