#include "Camera.h"

#include "Math.h"
#include "Mesh.h"
#include "Scene.h"
#include "Screen.h"
#include "Transform.h"
#include "GameObject.h"
#include "FileReader.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "AssetsManager.h"

bool Camera::s_inited = false;
Mesh* Camera::s_camMesh = nullptr;

void Camera::InitStatics()
{
    if (s_inited) return;

    s_camMesh = AssetsManager::Load<Mesh>("Meshes/Camera.bmesh", true);
    s_inited = true;
}

Camera::Camera()
{
    InitStatics();
}

Camera::~Camera()
{

}

void Camera::GetViewMatrix(Matrix4 *view) const
{
    if (m_identityMode) { *view = Matrix4::Identity; return; }

    transform->GetLocalToWorldMatrix(view);
    *view = view->Inversed();
}

void Camera::GetProjectionMatrix(Matrix4 *proj) const
{
    if (m_identityMode) { *proj = Matrix4::Identity; return; }

    if (m_projMode == ProjectionMode::Perspective)
    {
        if (m_autoUpdateAspectRatio)
        {
            m_aspectRatio = Screen::GetAspectRatio();
        }

        *proj = Matrix4::Perspective(
                    Math::Deg2Rad(m_fovDegrees), m_aspectRatio,
                    m_zNear, m_zFar);
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

Vector3 Camera::ScreenNDCPointToWorld(const Vector2 &screenNDCPos, float zFromCamera)
{
    Matrix4 p, v;
    GetProjectionMatrix(&p);
    GetViewMatrix(&v);

    // Pass coordinates to clip space, to invert them using projInversed
    Vector4 clipCoords = Vector4(screenNDCPos, 1.0, 1.0) * zFromCamera;
    Vector4 res4 = p.Inversed() * clipCoords;
    Vector3 res = res4.xyz();
    res = (v.Inversed() * Vector4(res, 1.0f)).xyz();
    return res;
}

String Camera::GetName() const
{
    return "Camera";
}

void Camera::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Camera *cam = Object::SCast<Camera>(clone);
    cam->SetZFar(GetZFar());
    cam->SetZNear(GetZNear());
    cam->SetClearColor(GetClearColor());
    cam->SetFovDegrees(GetFovDegrees());
    cam->SetOrthoHeight(GetOrthoHeight());
    cam->SetAspectRatio(GetAspectRatio());
    cam->SetProjectionMode(GetProjectionMode());
    cam->SetAutoUpdateAspectRatio(GetAutoUpdateAspectRatio());
}

ICloneable *Camera::Clone() const
{
    Camera *cam = new Camera();
    CloneInto(cam);
    return cam;
}

Rect Camera::GetScreenBoundingRect(const AABox &bbox)
{
    // If there's a point outside the camera rect, return Empty
    bool allPointsOutside = true;
    Rect screenRect = bbox.GetAABoundingScreenRect(this);
    Vector2 rMin = screenRect.GetMin(), rMax = screenRect.GetMax();
    allPointsOutside = allPointsOutside && !screenRect.Contains( Vector2(rMin.x, rMin.y) );
    allPointsOutside = allPointsOutside && !screenRect.Contains( Vector2(rMin.x, rMax.y) );
    allPointsOutside = allPointsOutside && !screenRect.Contains( Vector2(rMax.x, rMin.y) );
    allPointsOutside = allPointsOutside && !screenRect.Contains( Vector2(rMax.x, rMax.y) );
    if (allPointsOutside) { return Rect::Empty; }

    // If there's one or more points behind the camera, return ScreenRect
    // because we don't know how to handle it properly
    Array<Vector3> points = bbox.GetPoints();
    Vector3 camForward = transform->GetForward();
    for (const Vector3 &p : points)
    {
        Vector3 dirToP = p - transform->GetPosition();
        if (Vector3::Dot(dirToP, camForward) < 0) { return Rect::ScreenRect; }
    }

    return screenRect;
}

#ifdef BANG_EDITOR
void Camera::OnDrawGizmos()
{
    if (gameObject->HasHideFlag(HideFlags::HideInGame)) return;

    Component::OnDrawGizmos();

    Camera *sceneCam = SceneManager::GetActiveScene()->GetCamera();
    float distScale = Vector3::Distance(sceneCam->transform->GetPosition(),
                                        transform->GetPosition());

    Gizmos::SetDrawWireframe(false);
    Gizmos::SetReceivesLighting(true);
    Gizmos::SetPosition(transform->GetPosition());
    Gizmos::SetRotation(transform->GetRotation());
    Gizmos::SetScale(Vector3::One * 0.02f * distScale);
    Gizmos::SetColor(Color::White);
    Gizmos::RenderCustomMesh(Camera::s_camMesh);

    if (gameObject->IsSelected())
    {
        Gizmos::SetColor(Color::Red);
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
            AABox orthoBox;
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

void Camera::SetIdentityMode(bool identityMode)
{
    m_identityMode = identityMode;
}

bool Camera::IsInIdentityMode() const
{
    return m_identityMode;
}
