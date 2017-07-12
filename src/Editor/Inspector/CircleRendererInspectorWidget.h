#ifndef CIRCLECircleRendererINSPECTORWIDGET_H
#define CIRCLECircleRendererINSPECTORWIDGET_H

#include "Bang/CircleRenderer.h"
#include "Bang/RendererInspectorWidget.h"

class CircleRendererInspectorWidget : public RendererInspectorWidget
{
public:
    CircleRendererInspectorWidget(CircleRenderer *circleRenderer);
    virtual ~CircleRendererInspectorWidget();

protected:
    void InitExtra() override;
    void Refresh() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    void OnDestroy() override;

private:
    CircleRenderer *p_circleRenderer = nullptr;

    AttrWidgetFloat *m_radiusAW   = nullptr;
    AttrWidgetInt   *m_segmentsAW = nullptr;
};

#endif // CIRCLECircleRendererINSPECTORWIDGET_H
