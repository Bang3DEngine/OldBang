#ifndef POINTLIGHTINSPECTORWIDGET_H
#define POINTLIGHTINSPECTORWIDGET_H

#include "Bang/PointLight.h"
#include "Bang/LightInspectorWidget.h"

class PointLightInspectorWidget : public LightInspectorWidget
{
public:
    PointLightInspectorWidget(PointLight *pointLight);
    virtual ~PointLightInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    void OnDestroy() override;

private:
    PointLight *p_pointLight = nullptr;

    AttrWidgetFloat *m_rangeAW = nullptr;
};


#endif // POINTLIGHTINSPECTORWIDGET_H
