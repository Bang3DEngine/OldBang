#include "Bang/Camera.h"

#include "Bang/GL.h"
#include "Bang/Math.h"
#include "Bang/Mesh.h"
#include "Bang/Scene.h"
#include "Bang/G_Screen.h"
#include "Bang/Transform.h"
#include "Bang/GLContext.h"
#include "Bang/GameObject.h"
#include "Bang/FileReader.h"
#include "Bang/MeshRenderer.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"

Camera::Camera()
{
    p_camMesh = AssetsManager::Load<Mesh>( EPATH("Meshes/Camera.bmesh") );
}

Camera::~Camera()
{

}

void Camera::SetReplacementShaderProgram(G_ShaderProgram *replacementSP)
{
    m_replacementShaderProgram = replacementSP;
}

G_ShaderProgram *Camera::GetReplacementShaderProgram() const
{
    return m_replacementShaderProgram;
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
        *proj = Matrix4::Perspective(
                    Math::Deg2Rad(m_fovDegrees), G_Screen::GetAspectRatio(),
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

void Camera::Bind() const
{
    Matrix4 view, projection;
    GetViewMatrix(&view);
    GetProjectionMatrix(&projection);
    GL::SetZNearFar(GetZNear(), GetZFar());
    GL::SetViewMatrix(view);
    GL::SetProjectionMatrix(projection);
}

const Color &Camera::GetClearColor() const
{
    return m_clearColor;
}

float Camera::GetOrthoWidth() const
{
    return GetOrthoHeight() * G_Screen::GetAspectRatio();
}

float Camera::GetOrthoHeight() const
{
    return m_orthoHeight;
}


float Camera::GetFovDegrees() const
{
    return m_fovDegrees;
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

Vector2 Camera::WorldToScreenNDCPoint(const Vector3 &position)
{
    Matrix4 p, v;
    GetProjectionMatrix(&p);
    GetViewMatrix(&v);
    Vector4 v4 = p * v * Vector4(position, 1);
    v4 /= v4.w;

    return Vector2(v4.xy());
}

Vector3 Camera::ScreenNDCPointToWorld(const Vector2 &screenNDCPos,
                                      float zFromCamera)
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

void Camera::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Camera *cam = Object::SCast<Camera>(clone);
    cam->SetZFar(GetZFar());
    cam->SetZNear(GetZNear());
    cam->SetClearColor(GetClearColor());
    cam->SetFovDegrees(GetFovDegrees());
    cam->SetOrthoHeight(GetOrthoHeight());
    cam->SetProjectionMode(GetProjectionMode());
}

Rect Camera::GetScreenBoundingRect(const AABox &bbox)
{
    // If there's a point outside the camera rect, return Empty
    bool allPointsOutside = true;
    Rect screenRect = bbox.GetAABoundingScreenRect(this);
    Vector2 rMin = screenRect.GetMin(), rMax = screenRect.GetMax();
    allPointsOutside = allPointsOutside &&
                       !screenRect.Contains( Vector2(rMin.x, rMin.y) );
    allPointsOutside = allPointsOutside &&
                       !screenRect.Contains( Vector2(rMin.x, rMax.y) );
    allPointsOutside = allPointsOutside &&
                       !screenRect.Contains( Vector2(rMax.x, rMin.y) );
    allPointsOutside = allPointsOutside &&
                       !screenRect.Contains( Vector2(rMax.x, rMax.y) );
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
void Camera::OnDrawGizmos(bool depthed, bool overlay)
{
    if (gameObject->GetHideFlags()->IsOn(HideFlag::HideInGame)) return;

    Component::OnDrawGizmos(depthed, overlay);

    Camera *sceneCam = SceneManager::GetActiveScene()->GetCamera();
    float distScale = Vector3::Distance(sceneCam->transform->GetPosition(),
                                        transform->GetPosition());

    Gizmos::SetDrawWireframe(false);
    Gizmos::SetReceivesLighting(true);
    Gizmos::SetPosition(transform->GetPosition());
    Gizmos::SetRotation(transform->GetRotation());
    Gizmos::SetScale(Vector3::One * 0.02f * distScale);
    Gizmos::SetColor(Color::White);
    Gizmos::RenderCustomMesh(p_camMesh);

    if (depthed && gameObject->IsSelected())
    {
        Gizmos::SetColor(Color::Red);
        Gizmos::SetReceivesLighting(false);

        if (GetProjectionMode() == ProjectionMode::Perspective)
        {
            Gizmos::RenderFrustum(transform->GetForward(), transform->GetUp(),
                                  transform->GetPosition(),
                                  GetZNear(), GetZFar(),
                                  GetFovDegrees(), G_Screen::GetAspectRatio());
        }
        else
        {
            AABox orthoBox;
            orthoBox.SetMin(transform->GetPosition() +
                   Vector3(-GetOrthoWidth(), -GetOrthoHeight(), -GetZNear()));
            orthoBox.SetMax(transform->GetPosition() +
                   Vector3( GetOrthoWidth(), GetOrthoHeight(), -GetZFar()));
            Gizmos::SetRotation(transform->GetRotation());
            Gizmos::RenderSimpleBox(orthoBox);
        }
    }
}
#endif

void Camera::Read(const XMLNode &xmlInfo)
{
    Component::Read(xmlInfo);

    SetClearColor(xmlInfo.GetColor("ClearColor"));
    SetFovDegrees(xmlInfo.GetFloat("FOVDegrees"));
    SetZNear(xmlInfo.GetFloat("ZNear"));
    SetZFar(xmlInfo.GetFloat("ZFar"));
    ProjectionMode pm =
            xmlInfo.GetEnumSelectedName("ProjectionMode").Contains("Ortho") ?
                ProjectionMode::Orthographic : ProjectionMode::Perspective;
    SetProjectionMode(pm);
    SetOrthoHeight( xmlInfo.GetFloat("OrthoHeight") );
}

void Camera::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);

    xmlInfo->SetColor("ClearColor", GetClearColor());
    xmlInfo->SetFloat("ZNear", GetZNear());
    xmlInfo->SetFloat("ZFar", GetZFar());
    xmlInfo->SetEnum("ProjectionMode",
                     {"Orthographic", "Perspective"},
                     (m_projMode == ProjectionMode::Orthographic) ? 0 : 1,
                     {XMLProperty::Readonly});

    if (GetProjectionMode() == ProjectionMode::Orthographic)
    {
        xmlInfo->SetFloat("OrthoHeight", GetOrthoHeight());
        xmlInfo->SetFloat("FOVDegrees", GetFovDegrees(), {XMLProperty::Hidden});
    }
    else
    {
        xmlInfo->SetFloat("OrthoHeight", GetOrthoHeight(),
                          {XMLProperty::Hidden});
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
