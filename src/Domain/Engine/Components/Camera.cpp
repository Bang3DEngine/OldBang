#include "Camera.h"
#include "Canvas.h"
#include "FileReader.h"

Camera::Camera()
{
}

void Camera::GetViewMatrix(Matrix4 *view) const
{
    Transform *t = gameObject->transform;
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

void Camera::SetClearColor(const Color &color)
{
    m_clearColor = color;
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

const Color &Camera::GetClearColor() const
{
    return m_clearColor;
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

Vector2 Camera::WorldToScreenNDCPoint(const Vector3 &position)
{
    Matrix4 p, v;
    GetProjectionMatrix(&p);
    GetViewMatrix(&v);
    Vector4 v4 = p * v * Vector4(position, 1);
    v4 /= v4.w;

    return Vector2(v4.xy());
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

void Camera::OnDrawGizmos()
{
    Component::OnDrawGizmos();

    Texture2D *tex = AssetsManager::LoadAsset<Texture2D>("./Assets/Engine/Textures/CameraIcon.btex2d");
    Gizmos::SetColor(Color::gray);
    Gizmos::DrawIcon(tex, gameObject->transform->GetPosition(), Vector3::one * 10.0f);

    if (gameObject->IsSelectedInHierarchy())
    {
        Gizmos::SetColor(Color::red);
        Gizmos::SetReceivesLighting(false);

        if (GetProjectionMode() == ProjectionMode::Perspective)
        {
            Gizmos::DrawFrustum(transform->GetForward(), transform->GetUp(),
                                transform->GetPosition(),
                                GetZNear(), GetZFar(),
                                GetFovDegrees(), GetAspectRatio());
        }
        else
        {
            Box orthoBox;
            orthoBox.SetMin(Vector3(m_orthoRect.m_minx, m_orthoRect.m_miny, GetZNear()));
            orthoBox.SetMax(Vector3(m_orthoRect.m_maxx, m_orthoRect.m_maxy, GetZFar()));
            Gizmos::DrawSimpleBox(orthoBox);
        }
    }
}




#ifdef BANG_EDITOR
void Camera::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void Camera::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}
#endif

void Camera::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);

    SetClearColor(xmlInfo->GetColor("ClearColor"));
    SetFovDegrees(xmlInfo->GetFloat("FOVDegrees"));
    SetZNear(xmlInfo->GetFloat("ZNear"));
    SetZFar(xmlInfo->GetFloat("ZFar"));
    ProjectionMode pm = ProjectionMode_FromString(xmlInfo->GetEnumSelectedName("ProjectionMode"));
    SetProjectionMode(pm);
    SetOrthoRect( xmlInfo->GetRect("OrthoRectangle") );
}

void Camera::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Camera");

    xmlInfo->SetColor("ClearColor", GetClearColor());
    xmlInfo->SetFloat("FOVDegrees", GetFovDegrees());
    xmlInfo->SetFloat("ZNear", GetZNear());
    xmlInfo->SetFloat("ZFar", GetZFar());
    xmlInfo->SetEnum("ProjectionMode",
                     ProjectionMode_GetNamesVector(),
                     ProjectionMode_GetIndexFromValue(m_projMode),
                     {XMLProperty::Readonly});
    xmlInfo->SetRect("OrthoRectangle", m_orthoRect);
}
