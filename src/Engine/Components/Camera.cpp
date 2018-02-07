#include "Bang/Camera.h"

#include "Bang/GL.h"
#include "Bang/Math.h"
#include "Bang/Mesh.h"
#include "Bang/AABox.h"
#include "Bang/Scene.h"
#include "Bang/AARect.h"
#include "Bang/Gizmos.h"
#include "Bang/Window.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Matrix4.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Resources.h"
#include "Bang/Transform.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/GLUniforms.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/SelectionFramebuffer.h"

USING_NAMESPACE_BANG

Camera::Camera()
{
    AddRenderPass(RenderPass::Scene);
    AddRenderPass(RenderPass::ScenePostProcess);
    AddRenderPass(RenderPass::Canvas);
    AddRenderPass(RenderPass::CanvasPostProcess);

    m_gbuffer = new GBuffer(1,1);
    m_selectionFramebuffer = new SelectionFramebuffer(1,1);
}

Camera::~Camera()
{
    delete m_gbuffer;
    delete m_selectionFramebuffer;
}

void Camera::Bind() const
{
    GLUniforms::SetCameraUniforms(GetZNear(), GetZFar());
    GLUniforms::SetViewMatrix( GetViewMatrix() );
    GLUniforms::SetProjectionMatrix( GetProjectionMatrix() );

    SetViewportForRendering();
}

void Camera::UnBind() const
{
    GL::SetViewport(m_latestViewportRect);
    GetGBuffer()->UnBind();
    GetSelectionFramebuffer()->UnBind();
}

void Camera::SetViewportForBlitting() const
{
    m_latestViewportRect = AARecti( GetViewportRectInWindow() );
    GL::SetViewport(m_latestViewportRect);
}

void Camera::SetViewportForRendering() const
{
    SetViewportForBlitting();
    AARecti vpRect = GL::GetViewportRect();
    GL::SetViewport(0, 0, vpRect.GetWidth(), vpRect.GetHeight());
}

void Camera::BindGBuffer()
{
    Vector2i vpSize = GL::GetViewportSize();
    GetGBuffer()->Resize(vpSize.x, vpSize.y);

    GetGBuffer()->Bind();
    Color bgColor = GetClearColor();
    GetGBuffer()->ClearBuffersAndBackground(bgColor);
    GetGBuffer()->SetAllDrawBuffers();
}

void Camera::BindSelectionFramebuffer()
{
    Vector2i vpSize = GL::GetViewportSize();
    GetSelectionFramebuffer()->Resize(vpSize.x, vpSize.y);

    GetSelectionFramebuffer()->Bind();
    GL::ClearStencilBuffer();
    GetSelectionFramebuffer()->ClearDepth();
    GetSelectionFramebuffer()->ClearColor();
}

Ray Camera::FromViewportPointNDCToRay(const Vector2 &vpPointNDC) const
{
    Vector3 worldPoint = FromViewportPointNDCToWorldPoint(vpPointNDC, GetZNear());

    Ray ray;
    ray.SetOrigin( GetGameObject()->GetTransform()->GetPosition() );
    ray.SetDirection( (worldPoint - ray.GetOrigin()).Normalized() );
    return ray;
}

Vector2i Camera::FromWindowPointToViewportPoint(const Vector2i &winPoint) const
{
    return Vector2i(
                GL::FromWindowPointToViewportPoint(Vector2(winPoint),
                                                   AARecti(GetViewportRectInWindow())) );
}

Vector2 Camera::FromWorldPointToViewportPointNDC(const Vector3 &worldPosition) const
{
    Vector4 v4 = GetProjectionMatrix() *
                 GetViewMatrix() * Vector4(worldPosition, 1);
    v4 /= v4.w;
    return v4.xy();
}

Vector3 Camera::FromViewportPointNDCToWorldPoint(const Vector3 &vpPointNDC) const
{
    return FromViewportPointNDCToWorldPoint(vpPointNDC.xy(), vpPointNDC.z);
}

Vector3 Camera::FromViewportPointNDCToWorldPoint(const Vector2 &vpPointNDC,
                                                 float zFromCamera) const
{
    // Pass coordinates to clip space, to invert them using projInversed
    Vector4 clipCoords = Vector4(vpPointNDC, 1, 1) * zFromCamera;
    Vector4 res4 = GetProjectionMatrix().Inversed() * clipCoords;
    Vector3 res = res4.xyz();
    res = (GetViewMatrix().Inversed() * Vector4(res, 1)).xyz();
    return res;
}

AARect Camera::GetViewportBoundingRect(const AABox &bbox)
{
    // If there's a point outside the camera rect, return Empty
    bool allPointsOutside = true;
    AARect winRect = bbox.GetAABoundingViewportRect(this);
    Vector2 rMin = winRect.GetMin(), rMax = winRect.GetMax();
    allPointsOutside = allPointsOutside &&
                       !winRect.Contains( Vector2(rMin.x, rMin.y) );
    allPointsOutside = allPointsOutside &&
                       !winRect.Contains( Vector2(rMin.x, rMax.y) );
    allPointsOutside = allPointsOutside &&
                       !winRect.Contains( Vector2(rMax.x, rMin.y) );
    allPointsOutside = allPointsOutside &&
                       !winRect.Contains( Vector2(rMax.x, rMax.y) );
    if (allPointsOutside) { return AARect::Zero; }

    // If there's one or more points behind the camera, return WindowRect
    // because we don't know how to handle it properly
    Array<Vector3> points = bbox.GetPoints();
    Transform *tr = GetGameObject()->GetTransform();
    Vector3 camForward = tr->GetForward();
    for (const Vector3 &p : points)
    {
        Vector3 dirToP = p - tr->GetPosition();
        if (Vector3::Dot(dirToP, camForward) < 0) { return AARect::NDCRect; }
    }

    return winRect;
}

void Camera::SetOrthoHeight(float orthoHeight) { m_orthoHeight = orthoHeight; }
void Camera::SetClearColor(const Color &color) { m_clearColor = color; }
void Camera::SetFovDegrees(float fovDegrees) { this->m_fovDegrees = fovDegrees; }
void Camera::SetZNear(float zNear) { this->m_zNear = zNear; }
void Camera::SetZFar(float zFar) { this->m_zFar = zFar; }

void Camera::SetProjectionMode(Camera::ProjectionMode projMode)
{
    m_projMode = projMode;
}

void Camera::SetViewportRect(const AARect &viewportRectNDC)
{
    m_viewportRectNDC = viewportRectNDC;
}

void Camera::AddRenderPass(RenderPass renderPass)
{
    m_renderPassMask.Add(renderPass);
}

void Camera::RemoveRenderPass(RenderPass renderPass)
{
    m_renderPassMask.Remove(renderPass);
}

void Camera::SetRenderSelectionBuffer(bool renderSelectionBuffer)
{
    m_renderSelectionBuffer = renderSelectionBuffer;
}

const Color &Camera::GetClearColor() const { return m_clearColor; }
float Camera::GetOrthoHeight() const { return m_orthoHeight; }
float Camera::GetFovDegrees() const { return m_fovDegrees; }
float Camera::GetZNear() const { return m_zNear; }
float Camera::GetZFar() const { return m_zFar; }

bool Camera::MustRenderPass(RenderPass renderPass) const
{
    return GetRenderPassMask().Contains(renderPass);
}

const Set<RenderPass> &Camera::GetRenderPassMask() const
{
    return m_renderPassMask;
}

AARect Camera::GetViewportRectInWindow() const
{
    AARect vpRect = GetViewportRectNDC() * 0.5f + 0.5f;
    return AARect( vpRect * Vector2(GL::GetViewportSize())
                        + Vector2(GL::GetViewportRect().GetMin()) );
}
AARect Camera::GetViewportRectNDCInWindow() const
{
    return GL::FromWindowRectToWindowRectNDC( GetViewportRectInWindow() );
}

const AARect& Camera::GetViewportRectNDC() const
{
    return m_viewportRectNDC;
}

GBuffer *Camera::GetGBuffer() const
{
    return m_gbuffer;
}

SelectionFramebuffer *Camera::GetSelectionFramebuffer() const
{
    return m_selectionFramebuffer;
}

Camera *Camera::GetActive()
{
    Camera *cam = GEngine::GetCurrentRenderingCamera();
    if (!cam)
    {
        Scene *activeScene = SceneManager::GetActiveScene();
        cam = activeScene ? activeScene->GetCamera() : nullptr;
    }
    return cam;
}
Camera::ProjectionMode Camera::GetProjectionMode() const { return m_projMode; }

float Camera::GetOrthoWidth() const
{
   return GetOrthoHeight() * GL::GetViewportAspectRatio();
}

Matrix4 Camera::GetViewMatrix() const
{
    Transform *tr = GetGameObject()->GetTransform();
    return tr->GetLocalToWorldMatrix().Inversed();
}

bool Camera::GetRenderSelectionBuffer() const
{
    return m_renderSelectionBuffer;
}

Matrix4 Camera::GetProjectionMatrix() const
{
    if (m_projMode == ProjectionMode::Perspective)
    {
        if (GL::GetViewportAspectRatio() == 0.0 ||
            GetFovDegrees() == 0.0 ||
            GetZNear() == GetZFar())
        {
            return Matrix4::Identity;
        }

        return Matrix4::Perspective(Math::DegToRad(GetFovDegrees()),
                                    GL::GetViewportAspectRatio(),
                                    GetZNear(), GetZFar());
    }
    else //Ortho
    {
        return Matrix4::Ortho(-GetOrthoWidth(),  GetOrthoWidth(),
                              -GetOrthoHeight(), GetOrthoHeight(),
                               GetZNear(),       GetZFar());
    }
}

void Camera::OnRender(RenderPass rp)
{
    Component::OnRender(rp);
    if (rp != RenderPass::Overlay) { return; }

    Gizmos::Reset();
    static RH<Mesh> cameraMesh = MeshFactory::GetCamera();
    Transform *camTransform = GetGameObject()->GetTransform();
    float distScale = 15.0f;

    /*
    Camera *sceneCam = SceneManager::GetActiveScene()->GetCamera();
    Transform *sceneCamTransform = sceneCam->GetGameObject()->GetTransform();
    float distScale = Vector3::Distance(sceneCamTransform->GetPosition(),
                                        camTransform->GetPosition());
    */

    Gizmos::SetSelectable(GetGameObject());
    Gizmos::SetReceivesLighting(true);
    Gizmos::SetPosition(camTransform->GetPosition());
    Gizmos::SetRotation(camTransform->GetRotation());
    Gizmos::SetScale(Vector3::One * 0.02f * distScale);
    Gizmos::SetColor(Color::White);
    Gizmos::RenderCustomMesh(cameraMesh.Get());
}

void Camera::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Camera *cam = Cast<Camera*>(clone);
    cam->SetZFar(GetZFar());
    cam->SetZNear(GetZNear());
    cam->SetClearColor(GetClearColor());
    cam->SetFovDegrees(GetFovDegrees());
    cam->SetOrthoHeight(GetOrthoHeight());
    cam->SetProjectionMode(GetProjectionMode());
}

void Camera::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

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

void Camera::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("ClearColor", GetClearColor());
    xmlInfo->Set("ZNear", GetZNear());
    xmlInfo->Set("ZFar", GetZFar());
    xmlInfo->Set("ProjectionMode", GetProjectionMode());
    xmlInfo->Set("OrthoHeight", GetOrthoHeight());
    xmlInfo->Set("FOVDegrees", GetFovDegrees());
}
