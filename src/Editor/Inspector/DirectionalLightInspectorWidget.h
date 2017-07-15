#ifndef DIRECTIONALLIGHTINSPECTORWIDGET_H
#define DIRECTIONALLIGHTINSPECTORWIDGET_H

#include "Bang/DirectionalLight.h"
#include "Bang/LightInspectorWidget.h"

class DirectionalLightInspectorWidget : public LightInspectorWidget
{
public:
    DirectionalLightInspectorWidget(DirectionalLight *directionalLight);
    virtual ~DirectionalLightInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    void OnDestroy() override;

private:
    DirectionalLight *p_directionalLight = nullptr;
};


#endif // DIRECTIONALLIGHTINSPECTORWIDGET_H
