#include "Bang/CircleRendererInspectorWidget.h"

CircleRendererInspectorWidget::CircleRendererInspectorWidget(
        CircleRenderer *circleRenderer)
    : RendererInspectorWidget(circleRenderer)
{
    p_circleRenderer = circleRenderer;
}

CircleRendererInspectorWidget::~CircleRendererInspectorWidget()
{
}

void CircleRendererInspectorWidget::InitExtra()
{
    RendererInspectorWidget::InitExtra();

    m_radiusAW   = new AttrWidgetFloat("Radius");
    m_segmentsAW = new AttrWidgetInt("Num Segments");

    InsertAttributeWidget(m_radiusAW);
    InsertAttributeWidget(m_segmentsAW);
}

void CircleRendererInspectorWidget::Refresh()
{
    RendererInspectorWidget::Refresh();

    m_radiusAW->SetValue( p_circleRenderer->GetRadius() );
    m_segmentsAW->SetValue( p_circleRenderer->GetSegments() );
}

void CircleRendererInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    RendererInspectorWidget::OnAttrWidgetValueChanged(attrWidget);

    if (attrWidget == m_radiusAW)
    {
        p_circleRenderer->SetRadius( m_radiusAW->GetValue() );
    }
    else if (attrWidget == m_segmentsAW)
    {
        p_circleRenderer->SetDrawWireframe( m_segmentsAW->GetValue() );
    }
}

void CircleRendererInspectorWidget::OnDestroy()
{
    RendererInspectorWidget::OnDestroy();
}
