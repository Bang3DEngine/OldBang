#ifndef UIBORDERRECT_H
#define UIBORDERRECT_H

#include "Bang/Rect.h"
#include "Bang/Component.h"
#include "Bang/LineRenderer.h"

class UIBorderRect : public Component
{
public:
    UIBorderRect();
    virtual ~UIBorderRect();

    // Component
    virtual void OnUpdate() override;

    void SetLineColor(const Color& lineColor);
    void SetLineWidth(float lineWidth);

    const Color& GetLineColor() const;
    float GetLineWidth() const;

private:
    Rect m_latestBoundingRectNDC = Rect::Zero;
    LineRenderer *p_lineRenderer = nullptr;
};

#endif // UIBORDERRECT_H
