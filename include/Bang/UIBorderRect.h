#ifndef UIBORDERRECT_H
#define UIBORDERRECT_H

#include "Bang/Rect.h"
#include "Bang/Component.h"
#include "Bang/LineRenderer.h"

NAMESPACE_BANG_BEGIN

class UIBorderRect : public Component
{
public:
    UIBorderRect();
    virtual ~UIBorderRect();

    // Component
    virtual void OnStart() override;
    virtual void OnRender(RenderPass rp) override;

    void SetLineColor(const Color& lineColor);
    void SetLineWidth(float lineWidth);

    const Color& GetLineColor() const;
    float GetLineWidth() const;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    Rect m_latestBoundingRectNDC = Rect::Zero;
    LineRenderer *p_lineRenderer = nullptr;
};

NAMESPACE_BANG_END

#endif // UIBORDERRECT_H
