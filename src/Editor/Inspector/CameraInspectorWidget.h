#ifndef CAMERAINSPECTORWIDGET_H
#define CAMERAINSPECTORWIDGET_H

#include "Bang/Camera.h"
#include "Bang/ComponentWidget.h"

class CameraInspectorWidget : public ComponentWidget
{
public:
    CameraInspectorWidget(Camera *camera);
    virtual ~CameraInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    void OnDestroy() override;

private:
    Camera *p_camera = nullptr;

    AttrWidgetColor *m_clearColorAW  = nullptr;
    AttrWidgetFloat *m_zNearAW       = nullptr;
    AttrWidgetFloat *m_zFarAW        = nullptr;
    AttrWidgetFloat *m_orthoHeightAW = nullptr;
    AttrWidgetFloat *m_fovDegreesAW  = nullptr;
    AttrWidgetEnum<Camera::ProjectionMode>  *m_projModeAW    = nullptr;
};

#endif // CAMERAINSPECTORWIDGET_H
