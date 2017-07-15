#ifndef TRANSFORMINSPECTORWIDGET_H
#define TRANSFORMINSPECTORWIDGET_H

#include "Bang/Transform.h"
#include "Bang/ComponentWidget.h"

class TransformInspectorWidget : public ComponentWidget
{
public:
    TransformInspectorWidget(Transform *transform);
    virtual ~TransformInspectorWidget();

protected:
    virtual void InitExtra() override;
    virtual void OnUpdate() override;
    virtual void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    virtual void OnDestroy() override;

    Transform *p_transform = nullptr;

    AttrWidgetVectorFloat *m_positionAW = nullptr;
    AttrWidgetVectorFloat *m_rotationAW = nullptr;
    AttrWidgetVectorFloat *m_scaleAW    = nullptr;
};

#endif // TRANSFORMINSPECTORWIDGET_H
