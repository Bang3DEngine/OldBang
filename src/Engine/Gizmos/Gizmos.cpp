#include "Bang/Gizmos.h"

#include "Bang/Mesh.h"
#include "Bang/Math.h"
#include "Bang/Rect.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Renderer.h"
#include "Bang/G_Screen.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"
#include "Bang/CircleRenderer.h"
#include "Bang/GraphicPipeline.h"
#include "Bang/SingleLineRenderer.h"

Gizmos::Gizmos()
{
}


void Gizmos::Init()
{
    m_boxMesh    = MeshFactory::GetCube();
    m_planeMesh  = MeshFactory::GetPlane();
    m_sphereMesh = MeshFactory::GetSphere();

    Path matPath = EPATH("./Materials/G_DefaultNoSP.bmat");
    m_material   = AssetsManager::Load<Material>(matPath);
    m_material   = new Material(*m_material);

    m_gizmosGameObject   = new GameObject("GizmosGameObject");
    m_transform          = m_gizmosGameObject->transform;
    m_singleLineRenderer = m_gizmosGameObject->AddComponent<SingleLineRenderer>();
    m_circleRenderer     = m_gizmosGameObject->AddComponent<CircleRenderer>();
    m_meshRenderer       = m_gizmosGameObject->AddComponent<MeshRenderer>();
    m_renderers          = m_gizmosGameObject->GetComponents<Renderer>();

    for (Renderer *rend : m_renderers)
    {
        rend->SetMaterial(m_material);
    }
}

void Gizmos::SetResetAllowed(bool allowed)
{
    m_resetAllowed = allowed;
}

void Gizmos::SetColor(const Color &color)
{
    Gizmos *g = Gimzos::GetInstance();
    g->m_material->SetDiffuseColor(color);
}

void Gizmos::SetPosition(const Vector3 &position)
{
    Gizmos *g = Gimzos::GetInstance();
    g->m_transform->SetLocalPosition(position);
}

void Gizmos::SetRotation(const Quaternion &rotation)
{
    Gizmos *g = Gimzos::GetInstance();
    g->m_transform->SetLocalRotation(rotation);
}

void Gizmos::SetScale(const Vector3 &scale)
{
    Gizmos *g = Gimzos::GetInstance();
    g->m_transform->SetLocalScale(scale);
}

void Gizmos::SetLineWidth(float lineWidth)
{
    Gizmos *g = Gimzos::GetInstance();
    for (Renderer *rend : g->m_renderers)
    {
        rend->SetLineWidth(lineWidth);
    }
}

void Gizmos::SetDrawWireframe(bool wireframe)
{
    Gizmos *g = Gimzos::GetInstance();
    for (Renderer *rend : g->m_renderers)
    {
        rend->SetDrawWireframe(wireframe);
    }
}

void Gizmos::SetReceivesLighting(bool receivesLighting)
{
    Gizmos *g = Gimzos::GetInstance();
    for (Renderer *rend : g->m_renderers)
    {
        if (!rend->GetMaterial()) { continue; }
        rend->GetMaterial()->SetReceivesLighting(receivesLighting);
    }
}

void Gizmos::SetBillboard()
{
    Scene *scene = SceneManager::GetActiveScene();
    GameObject *cam = scene->GetCamera()->gameObject;
    Vector3 lookDir = (cam->transform->GetPosition() - m_transform->GetPosition());
    lookDir.Normalize();
    Gizmos::SetRotation( Quaternion::LookDirection(lookDir) );
}

void Gizmos::RenderCustomMesh(Mesh *m)
{
    ENSURE(m);
    Gizmos *g = Gizmos::GetInstance();
    g->m_meshRenderer->SetEnabled(true);
    g->m_meshRenderer->SetMesh(m);
    Render(g->m_meshRenderer);
    Reset();
}


void Gizmos::RenderSimpleBox(const AABox &b)
{
    Gizmos *g = Gizmos::GetInstance();
    const Quaternion &r = g->m_transform->GetLocalRotation();
    const Vector3& bMin = b.GetMin();
    const Vector3& bMax = b.GetMax();

    SetResetAllowed(false);
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
    SetResetAllowed(true);

    Reset();
}

void Gizmos::RenderBox(const AABox &b)
{
    m_meshRenderer->SetEnabled(true);
    m_meshRenderer->SetMesh(m_boxMesh);
    m_transform->SetPosition(b.GetCenter());
    m_transform->SetScale(m_transform->GetScale() * b.GetDimensions());
    Render(m_meshRenderer);
    Reset();
}

void Gizmos::RenderRect(const Rect &r)
{
    SetResetAllowed(false);
    RenderScreenLine( Vector2(r.m_minx, r.m_miny), Vector2(r.m_maxx, r.m_miny) );
    RenderScreenLine( Vector2(r.m_maxx, r.m_miny), Vector2(r.m_maxx, r.m_maxy) );
    RenderScreenLine( Vector2(r.m_maxx, r.m_maxy), Vector2(r.m_minx, r.m_maxy) );
    RenderScreenLine( Vector2(r.m_minx, r.m_maxy), Vector2(r.m_minx, r.m_miny) );
    SetResetAllowed(true);
    Reset();
}

void Gizmos::RenderFillRect(const Rect &r, const Color &fillColor)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_meshRenderer->SetMesh(g->m_planeMesh);

    Gizmos::SetPosition( Vector3(r.GetCenter(), 0) );
    Gizmos::SetScale( Vector3(r.GetSize(), 1) );

    Material *mat = g->m_meshRenderer->GetMaterial(); ENSURE(mat);
    mat->SetTexture(nullptr);
    mat->SetDiffuseColor(fillColor);

    GL::SetViewProjMode(GL::ViewProjMode::IgnoreBoth);
    Gizmos::Render(mr);
    Reset();
}

void Gizmos::RenderCircle(float radius)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_circleRenderer->SetRadius(radius);
    Gizmos::Render(cr);
    Reset();
}

void Gizmos::RenderIcon(const Texture2D *texture,
                                        bool billboard)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_meshRenderer->SetMesh(g->m_planeMesh);

    SetDrawWireframe(false);
    SetReceivesLighting(false);
    if (billboard)
    {
        Camera *cam = SceneManager::GetActiveScene()->GetCamera();

        Vector3 camPos = cam->transform->GetPosition();
        float distScale = 1.0f;
        if (cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
        {
           Vector3 pos = g->m_transform->GetPosition();
           distScale = Vector3::Distance(camPos, pos);
        }
        Vector3 scale = g->m_transform->GetScale();
        g->m_transform->SetScale(distScale * scale * 8.0f);

        g->m_transform->LookInDirection(cam->transform->GetForward(),
                                        cam->transform->GetUp());
    }
    mr->GetMaterial()->SetTexture(texture);
    Render(mr);
    Reset();
}

void Gizmos::RenderScreenIcon(const Texture2D *texture,
                                              const Rect &screenRect,
                                              bool fixAspectRatio)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_meshRenderer->SetMesh(g->m_planeMesh);

    Gizmos::SetPosition( Vector3(screenRect.GetCenter(), 0) );
    Gizmos::SetScale( Vector3(screenRect.GetSize(), 1) );

    g->m_meshRenderer->SetDrawWireframe(false);
    SetReceivesLighting(false);
    g->m_meshRenderer->GetMaterial()->SetTexture(texture);
    GL::SetViewProjMode(fixAspectRatio ? GL::ViewProjMode::OnlyFixAspectRatio :
                                         GL::ViewProjMode::IgnoreBoth);
    Render(mr);
    Reset();
}

void Gizmos::RenderLine(const Vector3 &origin,
                                        const Vector3 &destiny)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_singleLineRenderer->SetOrigin(origin);
    g->m_singleLineRenderer->SetDestiny(destiny);

    g->m_transform->SetPosition(Vector3::Zero);
    g->m_transform->SetScale(Vector3::One);

    Render(slr);
    Reset();
}

void Gizmos::RenderScreenLine(const Vector2 &origin,
                                              const Vector2 &destiny)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_singleLineRenderer->SetOrigin( Vector3(origin, 0) );
    g->m_singleLineRenderer->SetDestiny( Vector3(destiny, 0) );

    g->m_transform->SetPosition(Vector3::Zero);
    g->m_transform->SetScale(Vector3::One);

    GL::SetViewProjMode(GL::ViewProjMode::IgnoreBoth);
    Render(slr);
    Reset();
}

void Gizmos::RenderRay(const Vector3 &origin, const Vector3 &rayDir)
{
    RenderLine(origin, origin + rayDir);
    Reset();
}

void Gizmos::RenderSphere(const Vector3 &origin, float radius)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_meshRenderer->SetEnabled(true);
    g->m_meshRenderer->SetMesh(g->m_sphereMesh);
    g->m_transform->SetPosition(origin);
    g->m_transform->SetScale(radius);
    Render(mr);
    Reset();
}

void Gizmos::RenderFrustum(
       const Vector3 &forward, const Vector3 &up,
       const Vector3 &origin,
       float zNear, float zFar,
       float fovDegrees, float aspectRatio)
{
    const Vector3 &c = origin;
    const Vector3 right = Vector3::Cross(forward, up).Normalized();

    const float fovH = Math::Deg2Rad(fovDegrees) / 2.0f;

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

    SetResetAllowed(false);

    // Near plane
    RenderLine(nearUpLeft   , nearUpRight);
    RenderLine(nearUpRight  , nearDownRight);
    RenderLine(nearDownRight, nearDownLeft);
    RenderLine(nearDownLeft , nearUpLeft);

    // Far plane
    RenderLine(farUpLeft   , farUpRight);
    RenderLine(farUpRight  , farDownRight);
    RenderLine(farDownRight, farDownLeft);
    RenderLine(farDownLeft , farUpLeft);

    // Projection lines
    RenderLine(nearUpLeft   , farUpLeft);
    RenderLine(nearUpRight  , farUpRight);
    RenderLine(nearDownRight, farDownRight);
    RenderLine(nearDownLeft , farDownLeft);

    SetResetAllowed(true);
    Reset();
}

void Gizmos::RenderSimpleSphere(const Vector3 &origin,
                                                float radius)
{
    Gizmos *g = Gizmos::GetInstance();
    g->m_circleRenderer->SetEnabled(true);
    g->m_circleRenderer->SetRadius(radius);

    g->m_transform->SetPosition(origin);

    g->m_transform->SetLocalEuler(0, 0, 0);
    Render(g->m_circleRenderer);
    //g->m_transform->SetLocalEuler(0, 45, 0);
    //Render(g->m_circleRenderer);
    g->m_transform->SetLocalEuler(0, 90, 0);
    Render(g->m_circleRenderer);
    //g->m_transform->SetLocalEuler(0, 135, 0);
    //Render(g->m_circleRenderer);

    g->m_transform->SetLocalEuler(0, 0, 0);
    Render(g->m_circleRenderer);
    //g->m_transform->SetLocalEuler(45, 0, 0);
    //Render(g->m_circleRenderer);
    g->m_transform->SetLocalEuler(90, 0, 0);
    Render(g->m_circleRenderer);
    //g->m_transform->SetLocalEuler(135, 0, 0);
    //Render(g->m_circleRenderer);

    Reset();
}

void Gizmos::Reset()
{
    Gizmos *g = Gizmos::GetInstance();
    if (!g->m_resetAllowed) { return; }

    SetPosition(Vector3::Zero);
    SetRotation(Quaternion::Identity);
    SetScale(Vector3::One);
    SetColor(Color::Green);
    SetLineWidth(1.0f);
    SetReceivesLighting(false);
    SetDrawWireframe(false);
    GL::SetViewProjMode(GL::ViewProjMode::UseBoth);

    for (Renderer *rend : g->m_renderers)
    {
        rend->SetEnabled(false);
    }

    if (g->m_meshRenderer->GetMaterial())
    {
        g->m_meshRenderer->GetMaterial()->SetTexture(nullptr);
    }
}

void Gizmos::Render(Renderer *rend)
{
    rend->SetEnabled(true);

    GraphicPipeline *gp = GraphicPipeline::GetActive(); ENSURE(gp);
    gp->Render(rend);
}

Gizmos *Gizmos::GetInstance()
{
    return nullptr;
    /*
    Scene *scene = SceneManager::GetActiveScene();
    if (!scene) { return nullptr; }

    Gizmos *gizmosGo = scene->GetGizmos();
    gizmosGo->Init();
    return gizmosGo;
    */
}
