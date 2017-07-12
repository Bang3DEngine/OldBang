#ifndef LIGHTINSPECTORWIDGET_H
#define LIGHTINSPECTORWIDGET_H

#include "Bang/Light.h"
#include "Bang/ComponentWidget.h"

class LightInspectorWidget : public ComponentWidget
{
public:
    LightInspectorWidget(Light *light);
    virtual ~LightInspectorWidget();

protected:
    virtual void InitExtra() override;
    virtual void Refresh() override;
    virtual void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    virtual void OnDestroy() override;

    Light *p_light = nullptr;

    AttrWidgetFloat *m_intensityAW = nullptr;
    AttrWidgetColor *m_colorAW     = nullptr;
};

#endif // LIGHTINSPECTORWIDGET_H
