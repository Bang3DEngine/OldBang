#include "Bang/LightInspectorWidget.h"

LightInspectorWidget::LightInspectorWidget(Light *light)
    : ComponentWidget(light)
{
    p_light = light;
}

LightInspectorWidget::~LightInspectorWidget()
{
}

void LightInspectorWidget::InitExtra()
{
    ComponentWidget::InitExtra();

    m_intensityAW = new AttrWidgetFloat("Intensity");
    m_colorAW     = new AttrWidgetColor("Color");

    InsertAttributeWidget(m_intensityAW);
    InsertAttributeWidget(m_colorAW);
}

void LightInspectorWidget::OnUpdate()
{
    ComponentWidget::OnUpdate();

    m_intensityAW->SetValue( p_light->GetIntensity() );
    m_colorAW->SetValue( p_light->GetColor() );
}

void LightInspectorWidget::OnAttrWidgetValueChanged(IAttributeWidget *attrWidget)
{
    ComponentWidget::OnAttrWidgetValueChanged(attrWidget);

    if (attrWidget == m_intensityAW)
    {
        p_light->SetIntensity( m_intensityAW->GetValue() );
    }
    else if (attrWidget == m_colorAW)
    {
        p_light->SetColor( m_colorAW->GetValue() );
    }

    emit Changed(this);
}

void LightInspectorWidget::OnDestroy()
{
    ComponentWidget::OnDestroy();
}
