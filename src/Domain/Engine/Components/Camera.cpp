#include "Camera.h"
#include "Canvas.h"
#include "FileReader.h"

Camera::Camera() : m_orthoRect(Rect(-1.0f, 1.0f, -1.0f, 1.0f)),
                   m_fovDegrees(60.0f),
                   m_zNear(0.1f),
                   m_zFar(100.0f),
                   m_aspectRatio(1.0f),
                   m_projMode(ProjectionMode::Perspective),
                   m_autoUpdateAspectRatio(true)
{
    #ifdef BANG_EDITOR
    m_inspectorComponentInfo.AddSlotInfos(
    {
        new InspectorVFloatSWInfo( "FOV", {m_fovDegrees} ),
        new InspectorVFloatSWInfo( "Z Near", {m_zNear} ),
        new InspectorVFloatSWInfo( "Z Far", {m_zFar} ),
        new InspectorVFloatSWInfo( "Aspect Ratio", {m_aspectRatio} ),
        new InspectorEnumSWInfo( "Projection Mode", {"Orthographic", "Perspective"} )
    });
    #endif
}

void Camera::GetViewMatrix(Matrix4 *view) const
{
    Transform *t = gameObject->GetTransform();
    if (CAN_USE_COMPONENT(t))
    {
        t->GetModelMatrix(view);
        *view = view->Inversed();
    }
    else
    {
        Logger_Verbose(gameObject << " has a Camera but does not have a transform. " <<
                       "View matrix will be the idgameObject matrix.");

        *view = Matrix4(1.0f);
    }
}

void Camera::GetProjectionMatrix(Matrix4 *proj) const
{
    if (m_projMode == ProjectionMode::Perspective)
    {
        if (m_autoUpdateAspectRatio)
        {
            m_aspectRatio = Canvas::GetAspectRatio();
        }

        *proj = Matrix4::Perspective(glm::radians(m_fovDegrees), m_aspectRatio, m_zNear, m_zFar);
    }
    else //Ortho
    {
        *proj = Matrix4::Ortho(m_orthoRect.m_minx, m_orthoRect.m_maxx,
                               m_orthoRect.m_miny, m_orthoRect.m_maxy,
                               m_zNear, m_zFar);
    }
}

void Camera::SetOrthoRect(const Rect &rect)
{
    m_orthoRect = rect;
}







void Camera::SetFovDegrees(float fovDegrees)
{
    this->m_fovDegrees = fovDegrees;
}

void Camera::SetAspectRatio(float aspectRatio)
{
    this->m_aspectRatio = aspectRatio;
}

void Camera::SetZNear(float zNear)
{
    this->m_zNear = zNear;
}

void Camera::SetZFar(float zFar)
{
    this->m_zFar = zFar;
}

void Camera::SetProjectionMode(Camera::ProjectionMode projMode)
{
    this->m_projMode = projMode;
}

void Camera::SetAutoUpdateAspectRatio(bool autoUpdateAspectRatio)
{
    this->m_autoUpdateAspectRatio = autoUpdateAspectRatio;
}






float Camera::GetFovDegrees() const
{
    return m_fovDegrees;
}

float Camera::GetAspectRatio() const
{
    return m_aspectRatio;
}

float Camera::GetZNear() const
{
    return m_zNear;
}

float Camera::GetZFar() const
{
    return m_zFar;
}

Camera::ProjectionMode Camera::GetProjectionMode() const
{
    return m_projMode;
}

bool Camera::GetAutoUpdateAspectRatio() const
{
    return m_autoUpdateAspectRatio;
}

glm::vec2 Camera::WorldToScreenNDCPoint(const Vector3 &position)
{
    Matrix4 p, v;
    GetProjectionMatrix(&p);
    GetViewMatrix(&v);
    glm::vec4 v_4 = p * v * glm::vec4(position, 1);
    v_4 /= v_4.w;

    return v_4.xy();
}

const std::string Camera::ToString() const
{
    return "Camera";
}

void Camera::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Camera *cam = static_cast<Camera*>(clone);
    cam->SetAspectRatio(GetAspectRatio());
    cam->SetAutoUpdateAspectRatio(GetAutoUpdateAspectRatio());

}

ICloneable *Camera::Clone() const
{
    Camera *cam = new Camera();
    CloneInto(cam);
    return cam;
}


#ifdef BANG_EDITOR
InspectorWidgetInfo* Camera::GetComponentInfo()
{
    static_cast<InspectorVFloatSWInfo*>(m_inspectorComponentInfo.GetSlotInfo(0))->m_value = {m_fovDegrees};
    static_cast<InspectorVFloatSWInfo*>(m_inspectorComponentInfo.GetSlotInfo(1))->m_value = {m_zNear};
    static_cast<InspectorVFloatSWInfo*>(m_inspectorComponentInfo.GetSlotInfo(2))->m_value = {m_zFar};
    static_cast<InspectorVFloatSWInfo*>(m_inspectorComponentInfo.GetSlotInfo(3))->m_value = {m_aspectRatio};
    static_cast<InspectorEnumSWInfo*>(m_inspectorComponentInfo.GetSlotInfo(4))->selectedValueIndex = m_projMode;

    return &m_inspectorComponentInfo;
}

void Camera::OnSlotValueChanged(InspectorWidget *source)
{
    m_fovDegrees = source->GetSWVectorFloatValue("FOV")[0];
    m_zNear = source->GetSWVectorFloatValue("Z Near")[0];
    m_zFar = source->GetSWVectorFloatValue("Z Far")[0];
    m_aspectRatio = source->GetSWVectorFloatValue("Aspect Ratio")[0];
    m_projMode =  static_cast<Camera::ProjectionMode>(source->GetSWSelectedEnumIndex("Projection Mode"));
}
#endif

void Camera::WriteInternal(std::ostream &f) const
{
    Component::WriteInternal(f);
    f << ((void*)this) << std::endl;
    FileWriter::WriteFloat(m_fovDegrees, f);
    FileWriter::WriteFloat(m_zNear, f);
    FileWriter::WriteFloat(m_zFar, f);
    FileWriter::WriteString((m_projMode == ProjectionMode::Perspective ? "Perspective" : "Orthographic") , f);
    FileWriter::WriteRect(m_orthoRect, f);
}

void Camera::ReadInternal(std::istream &f)
{
    Component::ReadInternal(f);
    SetFovDegrees( FileReader::ReadFloat(f) );
    SetZNear( FileReader::ReadFloat(f) );
    SetZFar( FileReader::ReadFloat(f) );
    SetProjectionMode( FileReader::ReadString(f) == "Perspective" ?
                                            Camera::ProjectionMode::Perspective :
                                            Camera::ProjectionMode::Orthographic);
    SetOrthoRect( FileReader::ReadRect(f) );
}
