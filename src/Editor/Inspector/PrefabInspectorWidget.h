#ifndef PREFABINSPECTORWIDGET_H
#define PREFABINSPECTORWIDGET_H

#include "Bang/Prefab.h"
#include "Bang/PrefabFile.h"
#include "Bang/AssetInspectorWidget.h"

class PrefabInspectorWidget : public AssetInspectorWidget<Prefab>
{
public:
    PrefabInspectorWidget(const PrefabFile &prefabFile);
    virtual ~PrefabInspectorWidget();

protected:
    void InitExtra() override;
    void Refresh() override;
    void OnDestroy() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
};


#endif // PREFABINSPECTORWIDGET_H
