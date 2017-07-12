#include "LineRendererInspectorWidget.h"

LineRendererInspectorWidget::LineRendererInspectorWidget(
        LineRenderer *lineRenderer)
    : RendererInspectorWidget(lineRenderer)
{
    p_lineRenderer = lineRenderer;
}

LineRendererInspectorWidget::~LineRendererInspectorWidget()
{
}

void LineRendererInspectorWidget::InitExtra()
{
    RendererInspectorWidget::InitExtra();
}

void LineRendererInspectorWidget::Refresh()
{
    RendererInspectorWidget::Refresh();
}

void LineRendererInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    RendererInspectorWidget::OnAttrWidgetValueChanged(attrWidget);
}

void LineRendererInspectorWidget::OnDestroy()
{
    RendererInspectorWidget::OnDestroy();
}
