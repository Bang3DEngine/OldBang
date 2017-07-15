#ifndef CIRCLERENDERERINSPECTORWIDGET_H
#define CIRCLERENDERERINSPECTORWIDGET_H

#include "Bang/CircleRenderer.h"
#include "Bang/RendererInspectorWidget.h"

class CircleRendererInspectorWidget : public RendererInspectorWidget
{
public:
    CircleRendererInspectorWidget(CircleRenderer *circleRenderer);
    virtual ~CircleRendererInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    void OnDestroy() override;

private:
    CircleRenderer *p_circleRenderer = nullptr;

    AttrWidgetFloat *m_radiusAW   = nullptr;
    AttrWidgetInt   *m_segmentsAW = nullptr;
};

#endif // CIRCLERENDERERINSPECTORWIDGET_H
