#include "Bang/CameraInspectorWidget.h"

CameraInspectorWidget::CameraInspectorWidget(Camera *camera)
    : ComponentWidget(camera)
{
    p_camera = camera;
}

CameraInspectorWidget::~CameraInspectorWidget()
{

}

void CameraInspectorWidget::InitExtra()
{
    ComponentWidget::InitExtra();

    m_clearColorAW  = new AttrWidgetColor("Clear color");
    m_zNearAW       = new AttrWidgetFloat("ZNear");
    m_zFarAW        = new AttrWidgetFloat("ZFar");
    m_projModeAW    = new AttrWidgetEnum<Camera::ProjectionMode>
                                   ("Proj Mode",
                                    {Camera::ProjectionMode::Perspective,
                                     Camera::ProjectionMode::Orthographic},
                                    {"Perspective", "Orthographic"});
    m_orthoHeightAW = new AttrWidgetFloat("Ortho Height");
    m_fovDegreesAW  = new AttrWidgetFloat("FOV");

    InsertAttributeWidget(m_clearColorAW);
    InsertAttributeWidget(m_zNearAW);
    InsertAttributeWidget(m_zFarAW);
    InsertAttributeWidget(m_projModeAW);
    InsertAttributeWidget(m_orthoHeightAW);
    InsertAttributeWidget(m_fovDegreesAW);
}

void CameraInspectorWidget::OnUpdate()
{
    ComponentWidget::OnUpdate();

    m_clearColorAW->SetValue( p_camera->GetClearColor() );
    m_zNearAW->SetValue( p_camera->GetZNear() );
    m_zFarAW->SetValue( p_camera->GetZFar() );
    m_projModeAW->SetValue( p_camera->GetProjectionMode() );
    m_orthoHeightAW->SetValue( p_camera->GetOrthoHeight() );
    m_fovDegreesAW->SetValue( p_camera->GetFovDegrees() );

    const bool orthoMode =
            p_camera->GetProjectionMode() == Camera::ProjectionMode::Orthographic;
    m_orthoHeightAW->SetVisible(orthoMode);
    m_fovDegreesAW->SetVisible(!orthoMode);
}

void CameraInspectorWidget::OnAttrWidgetValueChanged(IAttributeWidget *attrWidget)
{
    ComponentWidget::OnAttrWidgetValueChanged(attrWidget);

    if (attrWidget == m_clearColorAW)
    {
        p_camera->SetClearColor( m_clearColorAW->GetValue() );
    }
    else if (attrWidget == m_zNearAW)
    {
        p_camera->SetZNear( m_zNearAW->GetValue() );
    }
    else if (attrWidget == m_zFarAW)
    {
        p_camera->SetZFar( m_zFarAW->GetValue() );
    }
    else if (attrWidget == m_projModeAW)
    {
        p_camera->SetProjectionMode( m_projModeAW->GetValue() );
    }
    else if (attrWidget == m_orthoHeightAW)
    {
        p_camera->SetOrthoHeight( m_orthoHeightAW->GetValue() );
    }
    else if (attrWidget == m_fovDegreesAW)
    {
        p_camera->SetFovDegrees( m_fovDegreesAW->GetValue() );
    }

    emit Changed(this);
}

void CameraInspectorWidget::OnDestroy()
{
    ComponentWidget::OnDestroy();
}
