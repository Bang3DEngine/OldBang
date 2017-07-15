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

void LineRendererInspectorWidget::OnUpdate()
{
    RendererInspectorWidget::OnUpdate();
}

void LineRendererInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    RendererInspectorWidget::OnAttrWidgetValueChanged(attrWidget);
    emit Changed(this);
}

void LineRendererInspectorWidget::OnDestroy()
{
    RendererInspectorWidget::OnDestroy();
}
