#ifndef RECTTRANSFORM_H
#define RECTTRANSFORM_H

#include "Bang/Vector2.h"
#include "Bang/Transform.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEnabledListener.h"
#include "Bang/IRectTransformListener.h"

NAMESPACE_BANG_BEGIN

class RectTransform : public Transform,
                      public IEnabledListener,
                      public IRectTransformListener,
                      public EventEmitter<IRectTransformListener>
{
    COMPONENT(RectTransform)

public:
    RectTransform();
    virtual ~RectTransform();

    void OnRenderGizmos() override;

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
    void SetMargins(const Vector2i& marginRightTop,
                    const Vector2i& marginLeftBot);
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
    Rect GetScreenSpaceRectPx() const;
    Rect GetScreenSpaceRectNDC() const;
    Rect GetParentScreenRectPx() const;
    Rect GetParentScreenRect() const;
    virtual const Matrix4& GetLocalToParentMatrix() const override;

    // Conversion functions
    Rect FromGlobalNDCToLocalNDC(const Rect &globalNDCRect) const;
    Vector2 FromGlobalNDCToLocalNDC(const Vector2 &globalNDCPoint) const;
    Rect FromLocalNDCToGlobalNDC(const Rect &localNDCRect) const;
    Vector2 FromLocalNDCToGlobalNDC(const Vector2 &localNDCPoint) const;
    Vector2 FromPixelsToLocalNDC(const Vector2i &pixels) const;
    Vector2 FromPixelsAmountToLocalNDC(const Vector2i &pixelsAmount) const;
    Vector2 FromLocalNDCToPixelsAmount(const Vector2 &ndcAmount) const;
    Vector2 FromPixelsPointToLocalNDC(const Vector2 &pixelsPoint) const;
    Vector2 FromPixelsPointToLocalNDC(const Vector2i &pixelsPoint) const;
    Vector2 FromLocalNDCToPixelsPoint(const Vector2 &ndcPoint) const;

    bool IsMouseOver(bool recursive = false) const;

    // IEnabledListener
    void OnEnabled() override;
    void OnDisabled() override;

    // IRectTransformListener
    void OnRectTransformChanged() override;
    void OnParentRectTransformChanged() override;
    void OnChildrenRectTransformChanged() override;

    // ICloneable
    void CloneInto(ICloneable *clone) const;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    // IInvalidatable
    void Invalidate() override;
    void OnInvalidated() override;

private:
    Vector2i m_marginRightTop = Vector2i::Zero;
    Vector2i m_marginLeftBot  = Vector2i::Zero;

    Vector2 m_pivotPosition =  Vector2::Zero;
    Vector2 m_anchorMin     = -Vector2::One;
    Vector2 m_anchorMax     =  Vector2::One;

    void PropagateParentRectTransformChangedEvent() const;
    void PropagateChildrenRectTransformChangedEvent() const;

    friend class UILayoutManager;
};

NAMESPACE_BANG_END

#endif // RECTTRANSFORM_H
