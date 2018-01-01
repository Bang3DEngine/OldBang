#include "Bang/Gizmos.h"

#include "Bang/Rect.h"
#include "Bang/Mesh.h"
#include "Bang/Math.h"
#include "Bang/Rect.h"
#include "Bang/AABox.h"
#include "Bang/Color.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/GEngine.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Renderer.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/Texture2D.h"
#include "Bang/Quaternion.h"
#include "Bang/GameObject.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/LineRenderer.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/MaterialFactory.h"
#include "Bang/RendererFactory.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

Gizmos::Gizmos()
{
    m_gizmosGo = GameObjectFactory::CreateGameObject();
    m_gizmosGo->SetName("Gizmos");

    p_boxMesh = Resources::Clone<Mesh>(MeshFactory::GetCube());
    p_planeMesh = Resources::Clone<Mesh>(MeshFactory::GetPlane());
    p_sphereMesh = Resources::Clone<Mesh>(MeshFactory::GetSphere());

    m_lineRenderer = m_gizmosGo->AddComponent<LineRenderer>();
    m_meshRenderer = m_gizmosGo->AddComponent<MeshRenderer>();
    m_renderers    = m_gizmosGo->GetComponents<Renderer>();

    for (Renderer *rend : m_renderers)
    {
        RendererFactory::ConvertToGizmoRenderer( rend );
    }

    m_gizmosGo->GetHideFlags().SetOn(HideFlag::DontSerialize);
}

Gizmos::~Gizmos()
{
    GameObject::Destroy(m_gizmosGo);
}

void Gizmos::SetColor(const Color &color)
{
    Gizmos *g = Gizmos::GetInstance();
    for (Renderer *rend : g->m_renderers)
    {
        rend->GetMaterial()->SetDiffuseColor(color);
    }
}

void Gizmos::SetPosition(const Vector3 &position)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_gizmosGo->GetTransform()->SetLocalPosition(position);
}

void Gizmos::SetRotation(const Quaternion &rotation)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_gizmosGo->GetTransform()->SetLocalRotation(rotation);
}

void Gizmos::SetScale(const Vector3 &scale)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_gizmosGo->GetTransform()->SetLocalScale(scale);
}

void Gizmos::SetThickness(float thickness)
{
    Gizmos *g = Gizmos::GetInstance();
    for (Renderer *rend : g->m_renderers)
    {
        rend->SetLineWidth(thickness);
        GL::PointSize(thickness);
    }
}

void Gizmos::SetRenderWireframe(bool wireframe)
{
    Gizmos *g = Gizmos::GetInstance();
    for (Renderer *rend : g->m_renderers)
    {
        rend->SetRenderWireframe(wireframe);
    }
}

void Gizmos::SetReceivesLighting(bool receivesLighting)
{
    Gizmos *g = Gizmos::GetInstance();
    for (Renderer *rend : g->m_renderers)
    {
        if (!rend->GetMaterial()) { continue; }
        rend->GetMaterial()->SetReceivesLighting(receivesLighting);
    }
}

void Gizmos::SetBillboard()
{
    Gizmos *g = Gizmos::GetInstance();
    Scene *scene = SceneManager::GetActiveScene();
    GameObject *cam = scene->GetCamera()->GetGameObject();
    Vector3 lookDir = (cam->GetTransform()->GetPosition() -
                       g->m_gizmosGo->GetTransform()->GetPosition());
    lookDir.Normalize();
    Gizmos::SetRotation( Quaternion::LookDirection(lookDir) );
}

void Gizmos::RenderCustomMesh(Mesh *m)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_meshRenderer->SetMesh(m);
    Render(g->m_meshRenderer);
}


void Gizmos::RenderSimpleBox(const AABox &b)
{
    Gizmos *g = Gizmos::GetInstance();
    const Quaternion &r = g->m_gizmosGo->GetTransform()->GetLocalRotation();
    const Vector3& bMin = b.GetMin();
    const Vector3& bMax = b.GetMax();

    RenderLine(r * Vector3(bMin.x, bMin.y, bMin.z), r * Vector3(bMax.x, bMin.y, bMin.z));
    RenderLine(r * Vector3(bMin.x, bMin.y, bMin.z), r * Vector3(bMin.x, bMax.y, bMin.z));
    RenderLine(r * Vector3(bMin.x, bMin.y, bMin.z), r * Vector3(bMin.x, bMin.y, bMax.z));

    RenderLine(r * Vector3(bMax.x, bMin.y, bMin.z), r * Vector3(bMax.x, bMax.y, bMin.z));
    RenderLine(r * Vector3(bMax.x, bMin.y, bMin.z), r * Vector3(bMax.x, bMin.y, bMax.z));

    RenderLine(r * Vector3(bMin.x, bMax.y, bMin.z), r * Vector3(bMax.x, bMax.y, bMin.z));
    RenderLine(r * Vector3(bMin.x, bMax.y, bMin.z), r * Vector3(bMin.x, bMax.y, bMax.z));

    RenderLine(r * Vector3(bMin.x, bMin.y, bMax.z), r * Vector3(bMax.x, bMin.y, bMax.z));
    RenderLine(r * Vector3(bMin.x, bMin.y, bMax.z), r * Vector3(bMin.x, bMax.y, bMax.z));

    RenderLine(r * Vector3(bMin.x, bMax.y, bMax.z), r * Vector3(bMax.x, bMax.y, bMax.z));
    RenderLine(r * Vector3(bMax.x, bMin.y, bMax.z), r * Vector3(bMax.x, bMax.y, bMax.z));
    RenderLine(r * Vector3(bMax.x, bMax.y, bMin.z), r * Vector3(bMax.x, bMax.y, bMax.z));
}

void Gizmos::RenderBox(const AABox &b)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_meshRenderer->SetMesh(g->p_boxMesh.Get());
    g->m_gizmosGo->GetTransform()->SetPosition(b.GetCenter());
    g->m_gizmosGo->GetTransform()->SetScale(g->m_gizmosGo->GetTransform()->GetScale() *
                                       b.GetDimensions());
    g->Render(g->m_meshRenderer);
}

void Gizmos::RenderRect(const Rect &r)
{
    RenderScreenLine( Vector2(r.m_min.x, r.m_min.y), Vector2(r.m_max.x, r.m_min.y) );
    RenderScreenLine( Vector2(r.m_max.x, r.m_min.y), Vector2(r.m_max.x, r.m_max.y) );
    RenderScreenLine( Vector2(r.m_max.x, r.m_max.y), Vector2(r.m_min.x, r.m_max.y) );
    RenderScreenLine( Vector2(r.m_min.x, r.m_max.y), Vector2(r.m_min.x, r.m_min.y) );
}

void Gizmos::RenderFillRect(const Rect &r)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_meshRenderer->SetMesh(g->p_planeMesh.Get());

    Gizmos::SetPosition( Vector3(r.GetCenter(), 0) );
    Gizmos::SetScale( Vector3(r.GetSize(), 1) );

    g->m_meshRenderer->SetViewProjMode(GL_ViewProjMode::IgnoreBoth);
    g->Render(g->m_meshRenderer);
}

void Gizmos::RenderIcon(Texture2D *texture, bool billboard)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_meshRenderer->SetMesh(g->p_planeMesh.Get());

    SetRenderWireframe(false);
    SetReceivesLighting(false);
    if (billboard)
    {
        Camera *cam = SceneManager::GetActiveScene()->GetCamera();

        Vector3 camPos = cam->GetGameObject()->GetTransform()->GetPosition();
        float distScale = 1.0f;
        if (cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
        {
           Vector3 pos = g->m_gizmosGo->GetTransform()->GetPosition();
           distScale = Vector3::Distance(camPos, pos);
        }
        Vector3 scale = g->m_gizmosGo->GetTransform()->GetScale();
        g->m_gizmosGo->GetTransform()->SetScale(distScale * scale * 8.0f);

        g->m_gizmosGo->GetTransform()->LookInDirection(
                    cam->GetGameObject()->GetTransform()->GetForward(),
                    cam->GetGameObject()->GetTransform()->GetUp());
    }
    g->m_meshRenderer->GetMaterial()->SetTexture(texture);
    g->Render(g->m_meshRenderer);
}

void Gizmos::RenderScreenIcon(Texture2D *texture,
                              const Rect &screenRect,
                              bool fixAspectRatio)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_meshRenderer->SetMesh(g->p_planeMesh.Get());

    Gizmos::SetPosition( Vector3(screenRect.GetCenter(), 0) );
    Gizmos::SetScale( Vector3(screenRect.GetSize(), 1) );

    g->m_meshRenderer->SetRenderWireframe(false);
    SetReceivesLighting(false);
    g->m_meshRenderer->GetMaterial()->SetTexture(texture);
    g->m_meshRenderer->SetViewProjMode(fixAspectRatio ?
                                        GL_ViewProjMode::OnlyFixAspectRatio :
                                        GL_ViewProjMode::IgnoreBoth);
    g->Render(g->m_meshRenderer);
}

void Gizmos::RenderLine(const Vector3 &origin, const Vector3 &destiny)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_lineRenderer->SetPoints( {origin, destiny} );

    g->m_gizmosGo->GetTransform()->SetPosition(Vector3::Zero);
    g->m_gizmosGo->GetTransform()->SetScale(Vector3::One);

    g->m_lineRenderer->SetViewProjMode(GL_ViewProjMode::UseBoth);
    g->Render(g->m_lineRenderer);
}

void Gizmos::RenderScreenLine(const Vector2 &origin, const Vector2 &destiny)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_lineRenderer->SetPoints( {Vector3(origin.x, origin.y, 0),
                                   Vector3(destiny.x, destiny.y, 0)} );

    g->m_gizmosGo->GetTransform()->SetPosition(Vector3::Zero);
    g->m_gizmosGo->GetTransform()->SetScale(Vector3::One);

    g->m_lineRenderer->SetViewProjMode(GL_ViewProjMode::IgnoreBoth);
    g->Render(g->m_lineRenderer);
}

void Gizmos::RenderRay(const Vector3 &origin, const Vector3 &rayDir)
{
    Gizmos::RenderLine(origin, origin + rayDir);
}

void Gizmos::RenderSphere(const Vector3 &origin, float radius)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_meshRenderer->SetMesh(g->p_sphereMesh.Get());
    g->m_gizmosGo->GetTransform()->SetPosition(origin);
    g->m_gizmosGo->GetTransform()->SetScale(radius);
    g->Render(g->m_meshRenderer);
}

void Gizmos::RenderFrustum(const Vector3 &forward, const Vector3 &up,
                           const Vector3 &origin,
                           float zNear, float zFar,
                           float fovDegrees, float aspectRatio)
{
    const Vector3 &c = origin;
    const Vector3 right = Vector3::Cross(forward, up).Normalized();

    const float fovH = Math::DegToRad(fovDegrees) / 2.0f;

    Vector3 nearPlaneCenter = c + forward * zNear;
    Vector3 farPlaneCenter  = c + forward * zFar;

    float nearHeight2 = zNear * Math::Tan(fovH);
    float nearWidth2 = nearHeight2 * aspectRatio;
    float farHeight2  = zFar  * Math::Tan(fovH);
    float farWidth2  = farHeight2 * aspectRatio;

    Vector3 nearUpLeft    = nearPlaneCenter - right * nearWidth2 + up * nearHeight2;
    Vector3 nearUpRight   = nearPlaneCenter + right * nearWidth2 + up * nearHeight2;
    Vector3 nearDownRight = nearPlaneCenter + right * nearWidth2 - up * nearHeight2;
    Vector3 nearDownLeft  = nearPlaneCenter - right * nearWidth2 - up * nearHeight2;

    Vector3 farUpLeft     = farPlaneCenter - right * farWidth2 + up * farHeight2;
    Vector3 farUpRight    = farPlaneCenter + right * farWidth2 + up * farHeight2;
    Vector3 farDownRight  = farPlaneCenter + right * farWidth2 - up * farHeight2;
    Vector3 farDownLeft   = farPlaneCenter - right * farWidth2 - up * farHeight2;

    // Near plane
    Gizmos::RenderLine(nearUpLeft   , nearUpRight);
    Gizmos::RenderLine(nearUpRight  , nearDownRight);
    Gizmos::RenderLine(nearDownRight, nearDownLeft);
    Gizmos::RenderLine(nearDownLeft , nearUpLeft);

    // Far plane
    Gizmos::RenderLine(farUpLeft   , farUpRight);
    Gizmos::RenderLine(farUpRight  , farDownRight);
    Gizmos::RenderLine(farDownRight, farDownLeft);
    Gizmos::RenderLine(farDownLeft , farUpLeft);

    // Projection lines
    Gizmos::RenderLine(nearUpLeft   , farUpLeft);
    Gizmos::RenderLine(nearUpRight  , farUpRight);
    Gizmos::RenderLine(nearDownRight, farDownRight);
    Gizmos::RenderLine(nearDownLeft , farDownLeft);
}

void Gizmos::RenderPoint(const Vector3 &point)
{
    Gizmos *g = Gizmos::GetInstance();

    RH<Mesh> rhm = Resources::Create<Mesh>();
    Mesh *m = rhm.Get();
    m->LoadPositions( {point} );

    g->m_gizmosGo->GetTransform()->SetPosition(Vector3::Zero);
    g->m_meshRenderer->SetMesh(m);
    g->m_meshRenderer->SetRenderPrimitive(GL_Primitive::Points);

    Render(g->m_meshRenderer);

    g->m_meshRenderer->SetRenderPrimitive(GL_Primitive::Triangles);
}

void Gizmos::Reset()
{
    Gizmos *g = Gizmos::GetInstance();

    Gizmos::SetPosition(Vector3::Zero);
    Gizmos::SetRotation(Quaternion::Identity);
    Gizmos::SetScale(Vector3::One);
    Gizmos::SetColor(Color::Green);
    Gizmos::SetThickness(1.0f);
    Gizmos::SetReceivesLighting(false);
    Gizmos::SetRenderWireframe(false);

    List<Renderer*> rends = g->m_gizmosGo->GetComponents<Renderer>();
    for (Renderer *rend : rends)
    {
        rend->SetViewProjMode(GL_ViewProjMode::UseBoth);
    }

    g->m_meshRenderer->GetMaterial()->SetTexture(nullptr);
}

GameObject *Gizmos::GetGameObject() const
{
    return m_gizmosGo;
}

GameObject *Gizmos::GetGizmosGameObject()
{
    return Gizmos::GetInstance()->GetGameObject();
}

void Gizmos::Render(Renderer *rend)
{
    rend->OnRender(RenderPass::Gizmos);
}

Gizmos* Gizmos::GetInstance()
{
    Scene *scene = SceneManager::GetActiveScene();
    return scene ? scene->GetGizmos() : nullptr;
}
