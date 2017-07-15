#include "Bang/DirectionalLightInspectorWidget.h"

DirectionalLightInspectorWidget::DirectionalLightInspectorWidget(
        DirectionalLight *directionalLight)
    : LightInspectorWidget(directionalLight)
{
    p_directionalLight = directionalLight;
}

DirectionalLightInspectorWidget::~DirectionalLightInspectorWidget()
{
}

void DirectionalLightInspectorWidget::InitExtra()
{
    LightInspectorWidget::InitExtra();
}

void DirectionalLightInspectorWidget::OnUpdate()
{
    LightInspectorWidget::OnUpdate();
}

void DirectionalLightInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    LightInspectorWidget::OnAttrWidgetValueChanged(attrWidget);
    emit Changed(this);
}

void DirectionalLightInspectorWidget::OnDestroy()
{
    LightInspectorWidget::OnDestroy();
}
