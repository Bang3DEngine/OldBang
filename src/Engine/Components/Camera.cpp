#include "Bang/Camera.h"

#include "Bang/GL.h"
#include "Bang/Rect.h"
#include "Bang/Math.h"
#include "Bang/Mesh.h"
#include "Bang/AABox.h"
#include "Bang/Screen.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Matrix4.h"
#include "Bang/Transform.h"
#include "Bang/ShaderProgram.h"
#include "Bang/Resources.h"

Camera::Camera()
{
    p_camMesh = Resources::Load<Mesh>( EPATH("Meshes/Camera.obj") );
}

Camera::~Camera()
{
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
    if (allPointsOutside) { return Rect::Zero; }

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

void Camera::SetOrthoHeight(float orthoHeight) { m_orthoHeight = orthoHeight; }
void Camera::SetClearColor(const Color &color) { m_clearColor = color; }
void Camera::SetFovDegrees(float fovDegrees) { this->m_fovDegrees = fovDegrees; }
void Camera::SetZNear(float zNear) { this->m_zNear = zNear; }
void Camera::SetZFar(float zFar) { this->m_zFar = zFar; }
void Camera::SetProjectionMode(Camera::ProjectionMode projMode)
{
    this->m_projMode = projMode;
}

const Color &Camera::GetClearColor() const { return m_clearColor; }
float Camera::GetOrthoHeight() const { return m_orthoHeight; }
float Camera::GetFovDegrees() const { return m_fovDegrees; }
float Camera::GetZNear() const { return m_zNear; }
float Camera::GetZFar() const { return m_zFar; }
Camera::ProjectionMode Camera::GetProjectionMode() const { return m_projMode; }

float Camera::GetOrthoWidth() const
{
   return GetOrthoHeight() * Screen::GetAspectRatio();
}

void Camera::GetViewMatrix(Matrix4 *view) const
{
    transform->GetLocalToWorldMatrix(view);
    *view = view->Inversed();
}

void Camera::GetProjectionMatrix(Matrix4 *proj) const
{
    if (m_projMode == ProjectionMode::Perspective)
    {
        *proj = Matrix4::Perspective(
                    Math::Deg2Rad(m_fovDegrees), Screen::GetAspectRatio(),
                    m_zNear, m_zFar);
    }
    else //Ortho
    {
        *proj = Matrix4::Ortho(-GetOrthoWidth(),  GetOrthoWidth(),
                               -GetOrthoHeight(), GetOrthoHeight(),
                                GetZNear(),       GetZFar());
    }
}

void Camera::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Camera *cam = SCAST<Camera*>(clone);
    cam->SetZFar(GetZFar());
    cam->SetZNear(GetZNear());
    cam->SetClearColor(GetClearColor());
    cam->SetFovDegrees(GetFovDegrees());
    cam->SetOrthoHeight(GetOrthoHeight());
    cam->SetProjectionMode(GetProjectionMode());
}

void Camera::Read(const XMLNode &xmlInfo)
{
    Component::Read(xmlInfo);

    if (xmlInfo.Contains("ClearColor"))
    { SetClearColor(xmlInfo.Get<Color>("ClearColor")); }

    if (xmlInfo.Contains("FOVDegrees"))
    { SetFovDegrees(xmlInfo.Get<float>("FOVDegrees")); }

    if (xmlInfo.Contains("ZNear"))
    { SetZNear(xmlInfo.Get<float>("ZNear")); }

    if (xmlInfo.Contains("ZFar"))
    { SetZFar(xmlInfo.Get<float>("ZFar")); }

    if (xmlInfo.Contains("ProjectionMode"))
    { SetProjectionMode( xmlInfo.Get<ProjectionMode>("ProjectionMode") ); }

    if (xmlInfo.Contains("OrthoHeight"))
    { SetOrthoHeight( xmlInfo.Get<float>("OrthoHeight") ); }
}

void Camera::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);

    xmlInfo->Set("ClearColor", GetClearColor());
    xmlInfo->Set("ZNear", GetZNear());
    xmlInfo->Set("ZFar", GetZFar());
    xmlInfo->Set("ProjectionMode", GetProjectionMode());
    xmlInfo->Set("OrthoHeight", GetOrthoHeight());
    xmlInfo->Set("FOVDegrees", GetFovDegrees());
}
