#ifndef LINERENDERERINSPECTORWIDGET_H
#define LINERENDERERINSPECTORWIDGET_H

#include "Bang/LineRenderer.h"
#include "Bang/RendererInspectorWidget.h"

class LineRendererInspectorWidget : public RendererInspectorWidget
{
public:
    LineRendererInspectorWidget(LineRenderer *lineRenderer);
    virtual ~LineRendererInspectorWidget();

protected:
    void InitExtra() override;
    void Refresh() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    void OnDestroy() override;

private:
    LineRenderer *p_lineRenderer = nullptr;
};
#endif // LINERENDERERINSPECTORWIDGET_H
