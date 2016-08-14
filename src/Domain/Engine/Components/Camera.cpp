#include "Camera.h"
#include "Canvas.h"
#include "FileReader.h"
#include "MeshRenderer.h"

bool Camera::s_inited = false;
Mesh* Camera::s_camMesh = nullptr;

void Camera::InitStatics()
{
    if (s_inited) return;

    s_camMesh = AssetsManager::LoadAsset<Mesh>(
                "Assets/Engine/Meshes/Camera.bmesh");

    s_inited = true;
}

Camera::Camera()
{
    InitStatics();
}

void Camera::GetViewMatrix(Matrix4 *view) const
{
    transform->GetModelMatrix(view);
    *view = view->Inversed();
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
        *proj = Matrix4::Ortho(-GetOrthoWidth(),  GetOrthoWidth(),
                               -GetOrthoHeight(), GetOrthoHeight(),
                                GetZNear(),       GetZFar());
    }
}

void Camera::SetOrthoHeight(float orthoHeight)
{
    m_orthoHeight = orthoHeight;
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

float Camera::GetOrthoWidth() const
{
    return GetOrthoHeight() * GetAspectRatio();
}

float Camera::GetOrthoHeight() const
{
    return m_orthoHeight;
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

#ifdef BANG_EDITOR
void Camera::OnDrawGizmos()
{
    if (gameObject->IsEditorGameObject()) return;

    Component::OnDrawGizmos();

    Gizmos::SetReceivesLighting(true);
    Gizmos::SetPosition(transform->GetPosition());
    Gizmos::SetRotation(transform->GetRotation());
    Gizmos::SetScale(Vector3::one * 0.015f);
    Gizmos::SetColor(GetClearColor());
    Gizmos::RenderCustomMesh(Camera::s_camMesh);

    if (gameObject->IsSelectedInHierarchy())
    {
        Gizmos::SetColor(Color::red);
        Gizmos::SetReceivesLighting(false);

        if (GetProjectionMode() == ProjectionMode::Perspective)
        {
            Gizmos::RenderFrustum(transform->GetForward(), transform->GetUp(),
                                  transform->GetPosition(),
                                  GetZNear(), GetZFar(),
                                  GetFovDegrees(), GetAspectRatio());
        }
        else
        {
            Box orthoBox;
            orthoBox.SetMin(transform->GetPosition() + Vector3(-GetOrthoWidth(), -GetOrthoHeight(), -GetZNear()));
            orthoBox.SetMax(transform->GetPosition() + Vector3( GetOrthoWidth(),  GetOrthoHeight(), -GetZFar()));
            Gizmos::SetRotation(transform->GetRotation());
            Gizmos::RenderSimpleBox(orthoBox);
        }
    }
}


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
    SetAspectRatio( xmlInfo->GetFloat("AspectRatio") );
    SetOrthoHeight( xmlInfo->GetFloat("OrthoHeight") );
}

void Camera::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Camera");

    xmlInfo->SetColor("ClearColor", GetClearColor());
    xmlInfo->SetFloat("ZNear", GetZNear());
    xmlInfo->SetFloat("ZFar", GetZFar());
    xmlInfo->SetEnum("ProjectionMode",
                     ProjectionMode_GetNamesVector(),
                     ProjectionMode_GetIndexFromValue(m_projMode),
                     {XMLProperty::Readonly});
    xmlInfo->SetFloat("AspectRatio", GetAspectRatio());

    if (GetProjectionMode() == ProjectionMode::Orthographic)
    {
        xmlInfo->SetFloat("OrthoHeight", GetOrthoHeight());
        xmlInfo->SetFloat("FOVDegrees", GetFovDegrees(), {XMLProperty::Hidden});
    }
    else
    {
        xmlInfo->SetFloat("OrthoHeight", GetOrthoHeight(), {XMLProperty::Hidden});
        xmlInfo->SetFloat("FOVDegrees", GetFovDegrees());
    }
}
