#ifndef RECTTRANSFORMINSPECTORWIDGET_H
#define RECTTRANSFORMINSPECTORWIDGET_H

#include "Bang/RectTransform.h"
#include "Bang/ComponentWidget.h"

class RectTransformInspectorWidget : public ComponentWidget
{
public:
    RectTransformInspectorWidget(RectTransform *rectTransform);
    virtual ~RectTransformInspectorWidget();

protected:
    virtual void InitExtra() override;
    virtual void OnUpdate() override;
    virtual void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    virtual void OnDestroy() override;

    RectTransform *p_rectTransform = nullptr;

    AttrWidgetInt *m_marginLeftAW  = nullptr;
    AttrWidgetInt *m_marginTopAW   = nullptr;
    AttrWidgetInt *m_marginRightAW = nullptr;
    AttrWidgetInt *m_marginBotAW   = nullptr;

    AttrWidgetVectorFloat *m_pivotPositionAW = nullptr;
    AttrWidgetVectorFloat *m_anchorMinAW     = nullptr;
    AttrWidgetVectorFloat *m_anchorMaxAW     = nullptr;
};

#endif // RECTTRANSFORMINSPECTORWIDGET_H
