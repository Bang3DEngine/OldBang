#include "Bang/PrefabInspectorWidget.h"

PrefabInspectorWidget::PrefabInspectorWidget(const PrefabFile &prefabFile)
    : AssetInspectorWidget(prefabFile)
{
}

PrefabInspectorWidget::~PrefabInspectorWidget()
{

}
void PrefabInspectorWidget::OnDestroy()
{
    AssetInspectorWidget::OnDestroy();
}

void PrefabInspectorWidget::InitExtra()
{
    AssetInspectorWidget::InitExtra();
}

void PrefabInspectorWidget::Refresh()
{
    AssetInspectorWidget::Refresh();
}

void PrefabInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    AssetInspectorWidget::OnAttrWidgetValueChanged(attrWidget);
}
