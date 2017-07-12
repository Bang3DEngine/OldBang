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

void DirectionalLightInspectorWidget::Refresh()
{
    LightInspectorWidget::Refresh();
}

void DirectionalLightInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    LightInspectorWidget::OnAttrWidgetValueChanged(attrWidget);
}

void DirectionalLightInspectorWidget::OnDestroy()
{
    LightInspectorWidget::OnDestroy();
}
