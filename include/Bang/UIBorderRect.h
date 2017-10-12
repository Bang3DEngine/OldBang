#ifndef UIBORDERRECT_H
#define UIBORDERRECT_H

#include "Bang/Rect.h"
#include "Bang/Component.h"
#include "Bang/LineRenderer.h"
#include "Bang/IRectTransformListener.h"

NAMESPACE_BANG_BEGIN

class UIBorderRect : public Renderer,
                     public IRectTransformListener
{
public:
    UIBorderRect();
    virtual ~UIBorderRect();

    // Component
    virtual void OnStart() override;

    void SetLineColor(const Color& lineColor);
    void SetLineWidth(float lineWidth);
    void SetPadding(int paddingPx);

    const Color& GetLineColor() const;
    float GetLineWidth() const;
    int GetPaddingPx() const;

    virtual void OnRectTransformChanged() override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    Rect m_latestBoundingRectNDC = Rect::Zero;
    LineRenderer *p_lineRenderer = nullptr;
    int m_padding = 0;
};

NAMESPACE_BANG_END

#endif // UIBORDERRECT_H
