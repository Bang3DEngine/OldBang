#include "Gizmos.h"

#include "Rect.h"
#include "Debug.h"
#include "Scene.h"
#include "Camera.h"
#include "Renderer.h"
#include "Material.h"
#include "Transform.h"
#include "Texture2D.h"
#include "EditorScene.h"
#include "MeshFactory.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "AssetsManager.h"
#include "CircleRenderer.h"
#include "GraphicPipeline.h"
#include "EditorGameObject.h"
#include "SingleLineRenderer.h"

Array<Renderer*> Gizmos::m_renderers;

Mesh *Gizmos::m_boxMesh = nullptr;
Mesh *Gizmos::m_sphereMesh = nullptr;
Mesh *Gizmos::m_planeMesh = nullptr;
Material *Gizmos::m_material = nullptr;

bool Gizmos::m_inited = false;
bool Gizmos::m_resetAllowed = true;

void Gizmos::Init()
{
    if (!Gizmos::m_inited)
    {
        Gizmos::m_boxMesh = MeshFactory::GetCube();
        Gizmos::m_sphereMesh = MeshFactory::GetSphere();
        Gizmos::m_planeMesh = MeshFactory::GetPlane();
        Gizmos::m_material = AssetsManager::Load<Material>("Materials/D2G_Default.bmat", true);
        Gizmos::m_material = new Material(*Gizmos::m_material);
        Gizmos::m_inited = true;
    }
}

void Gizmos::InitGizmosGameObject(EditorGameObject *gizmosGo)
{
    if (!gizmosGo)
    {
        m_renderers.Clear();
        return;
    }

    Gizmos::Init();

    SingleLineRenderer *r1 = gizmosGo->AddComponent<SingleLineRenderer>();
    CircleRenderer *r2 = gizmosGo->AddComponent<CircleRenderer>();
    MeshRenderer *r3 = gizmosGo->AddComponent<MeshRenderer>();

    m_renderers.Clear();
    m_renderers.PushBack(r1);
    m_renderers.PushBack(r2);
    m_renderers.PushBack(r3);

    for (Renderer *rend : Gizmos::m_renderers)
    {
        rend->SetIsGizmo(true);
        rend->SetMaterial(Gizmos::m_material);
        rend->SetDepthLayer(Renderer::DepthLayer::DepthLayerGizmos);
    }
}

EditorGameObject *Gizmos::GetGizmosGameObject()
{
    Scene *scene = SceneManager::GetActiveScene();
    ASSERT(scene, "", return nullptr);

    EditorScene *edScene = static_cast<EditorScene*>(scene);
    ASSERT(edScene, "", return nullptr);

    return edScene->GetGizmosGameObject();
}

void Gizmos::SetResetAllowed(bool allowed)
{
    m_resetAllowed = allowed;
}

void Gizmos::SetColor(const Color &color)
{
    Gizmos::m_material->SetDiffuseColor(color);
}

void Gizmos::SetPosition(const Vector3 &position)
{
    Gizmos::Gizmos::GetGizmosGameObject()->transform->SetLocalPosition(position);
}

void Gizmos::SetRotation(const Quaternion &rotation)
{
    Gizmos::Gizmos::GetGizmosGameObject()->transform->SetLocalRotation(rotation);
}

void Gizmos::SetScale(const Vector3 &scale)
{
    Gizmos::Gizmos::GetGizmosGameObject()->transform->SetLocalScale(scale);
}

void Gizmos::SetLineWidth(float lineWidth)
{
    for (Renderer *rend : Gizmos::m_renderers)
    {
        rend->SetLineWidth(lineWidth);
    }
}

void Gizmos::SetDrawWireframe(bool wireframe)
{
    for (Renderer *rend : Gizmos::m_renderers)
    {
        rend->SetDrawWireframe(wireframe);
    }
}

void Gizmos::SetReceivesLighting(bool receivesLighting)
{
    for (Renderer *rend : Gizmos::m_renderers)
    {
        rend->SetReceivesLighting(receivesLighting);
    }
}

void Gizmos::RenderCustomMesh(Mesh *m)
{
    GameObject *gizmosGo = Gizmos::GetGizmosGameObject();
    ASSERT(m); ASSERT(gizmosGo);

    MeshRenderer *mr = gizmosGo->GetComponent<MeshRenderer>();
    mr->SetEnabled(true);
    mr->SetMesh(m);
    Gizmos::Render(mr);

    Gizmos::Reset();
}


void Gizmos::RenderSimpleBox(const AABox &b)
{
    ASSERT(Gizmos::GetGizmosGameObject());

    const Quaternion &r = Gizmos::GetGizmosGameObject()->transform->GetLocalRotation();
    const Vector3& bMin = b.GetMin();
    const Vector3& bMax = b.GetMax();

    Gizmos::SetResetAllowed(false);
    Gizmos::RenderLine(r * Vector3(bMin.x, bMin.y, bMin.z), r * Vector3(bMax.x, bMin.y, bMin.z));
    Gizmos::RenderLine(r * Vector3(bMin.x, bMin.y, bMin.z), r * Vector3(bMin.x, bMax.y, bMin.z));
    Gizmos::RenderLine(r * Vector3(bMin.x, bMin.y, bMin.z), r * Vector3(bMin.x, bMin.y, bMax.z));

    Gizmos::RenderLine(r * Vector3(bMax.x, bMin.y, bMin.z), r * Vector3(bMax.x, bMax.y, bMin.z));
    Gizmos::RenderLine(r * Vector3(bMax.x, bMin.y, bMin.z), r * Vector3(bMax.x, bMin.y, bMax.z));

    Gizmos::RenderLine(r * Vector3(bMin.x, bMax.y, bMin.z), r * Vector3(bMax.x, bMax.y, bMin.z));
    Gizmos::RenderLine(r * Vector3(bMin.x, bMax.y, bMin.z), r * Vector3(bMin.x, bMax.y, bMax.z));

    Gizmos::RenderLine(r * Vector3(bMin.x, bMin.y, bMax.z), r * Vector3(bMax.x, bMin.y, bMax.z));
    Gizmos::RenderLine(r * Vector3(bMin.x, bMin.y, bMax.z), r * Vector3(bMin.x, bMax.y, bMax.z));

    Gizmos::RenderLine(r * Vector3(bMin.x, bMax.y, bMax.z), r * Vector3(bMax.x, bMax.y, bMax.z));
    Gizmos::RenderLine(r * Vector3(bMax.x, bMin.y, bMax.z), r * Vector3(bMax.x, bMax.y, bMax.z));
    Gizmos::RenderLine(r * Vector3(bMax.x, bMax.y, bMin.z), r * Vector3(bMax.x, bMax.y, bMax.z));
    Gizmos::SetResetAllowed(true);

    Gizmos::Reset();
}

void Gizmos::RenderBox(const AABox &b)
{
    ASSERT(Gizmos::GetGizmosGameObject());

    EditorGameObject *gizmosGo = Gizmos::GetGizmosGameObject();

    MeshRenderer *mr = gizmosGo->GetComponent<MeshRenderer>();
    mr->SetEnabled(true);
    mr->SetMesh(Gizmos::m_boxMesh);

    gizmosGo->transform->SetPosition(b.GetCenter());
    gizmosGo->transform->SetScale(
                gizmosGo->transform->GetScale() * b.GetDimensions());

    Gizmos::Render(mr);

    Gizmos::Reset();
}

void Gizmos::RenderRect(const Rect &r)
{
    ASSERT(Gizmos::GetGizmosGameObject());
    Gizmos::SetResetAllowed(false);
    Gizmos::RenderScreenLine( Vector2(r.m_minx, r.m_miny), Vector2(r.m_maxx, r.m_miny) );
    Gizmos::RenderScreenLine( Vector2(r.m_maxx, r.m_miny), Vector2(r.m_maxx, r.m_maxy) );
    Gizmos::RenderScreenLine( Vector2(r.m_maxx, r.m_maxy), Vector2(r.m_minx, r.m_maxy) );
    Gizmos::RenderScreenLine( Vector2(r.m_minx, r.m_maxy), Vector2(r.m_minx, r.m_miny) );
    Gizmos::SetResetAllowed(true);
    Gizmos::Reset();
}

void Gizmos::RenderIcon(const Texture2D *texture,
                        bool billboard)
{
    EditorGameObject *gizmosGo = Gizmos::GetGizmosGameObject();
    ASSERT(gizmosGo);

    MeshRenderer *mr = gizmosGo->GetComponent<MeshRenderer>();
    ASSERT(mr);

    ASSERT(Gizmos::m_planeMesh);

    Gizmos::SetDrawWireframe(false);
    Gizmos::SetReceivesLighting(false);

    mr->SetMesh(Gizmos::m_planeMesh);

    if (billboard)
    {
        Camera *cam = Scene::GetCamera();
        Vector3 camPos = cam->transform->GetPosition();

        float distScale = 1.0f;
        if (cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
        {
           Vector3 pos = Gizmos::Gizmos::GetGizmosGameObject()->transform->GetPosition();
           distScale = Vector3::Distance(camPos, pos);
        }

        Vector3 scale = Gizmos::Gizmos::GetGizmosGameObject()->transform->GetScale();
        gizmosGo->transform->SetScale(distScale * scale);
        gizmosGo->transform->LookInDirection(cam->transform->GetForward(),
                                             cam->transform->GetUp());
    }
    mr->GetMaterial()->SetTexture(texture);
    Gizmos::Render(mr);

    Gizmos::Reset();
}

void Gizmos::RenderLine(const Vector3 &origin, const Vector3 &destiny)
{
    ASSERT(Gizmos::GetGizmosGameObject());

    EditorGameObject *gizmosGo = Gizmos::GetGizmosGameObject();
    SingleLineRenderer *slr = gizmosGo->GetComponent<SingleLineRenderer>();
    slr->SetOrigin(origin);
    slr->SetDestiny(destiny);

    gizmosGo->transform->SetPosition(Vector3::Zero);
    gizmosGo->transform->SetScale(Vector3::One);

    Gizmos::Render(slr);

    Gizmos::Reset();
}

void Gizmos::RenderScreenLine(const Vector2 &origin, const Vector2 &destiny)
{
    ASSERT(Gizmos::GetGizmosGameObject());
    Camera *cam = SceneManager::GetActiveScene()->GetCamera(); ASSERT(cam);
    const float z = cam->GetZNear() + 0.01f;
    Vector3 worldPosOrigin  = cam->ScreenNDCPointToWorld(origin,  z);
    Vector3 worldPosDestiny = cam->ScreenNDCPointToWorld(destiny, z);
    bool resetAllowedBefore = m_resetAllowed;
    Gizmos::SetResetAllowed(false);
    Gizmos::RenderLine(worldPosOrigin, worldPosDestiny);
    Gizmos::SetResetAllowed(resetAllowedBefore);
    Gizmos::Reset();
}

void Gizmos::RenderRay(const Vector3 &origin, const Vector3 &rayDir)
{
    Gizmos::RenderLine(origin, origin + rayDir);
    Gizmos::Reset();
}

void Gizmos::RenderSphere(const Vector3 &origin, float radius)
{
    ASSERT(Gizmos::GetGizmosGameObject());

    EditorGameObject *gizmosGo = Gizmos::GetGizmosGameObject();
    MeshRenderer *mr = gizmosGo->GetComponent<MeshRenderer>();
    mr->SetEnabled(true);
    mr->SetMesh(Gizmos::m_sphereMesh);

    gizmosGo->transform->SetPosition(origin);
    gizmosGo->transform->SetScale(radius);

    Gizmos::Render(mr);

    Gizmos::Reset();
}

void Gizmos::RenderFrustum(const Vector3 &forward, const Vector3 &up,
                           const Vector3 &origin,
                           float zNear, float zFar,
                           float fovDegrees, float aspectRatio)
{
    ASSERT(Gizmos::GetGizmosGameObject());

    const Vector3 &c = origin;
    const Vector3 right = Vector3::Cross(forward, up).Normalized();

    const float fovH = glm::radians(fovDegrees) / 2.0f;

    Vector3 nearPlaneCenter = c + forward * zNear;
    Vector3 farPlaneCenter  = c + forward * zFar;

    float nearHeight2 = zNear * glm::tan(fovH);
    float nearWidth2 = nearHeight2 * aspectRatio;
    float farHeight2  = zFar  * glm::tan(fovH);
    float farWidth2  = farHeight2 * aspectRatio;

    Vector3 nearUpLeft    = nearPlaneCenter - right * nearWidth2 + up * nearHeight2;
    Vector3 nearUpRight   = nearPlaneCenter + right * nearWidth2 + up * nearHeight2;
    Vector3 nearDownRight = nearPlaneCenter + right * nearWidth2 - up * nearHeight2;
    Vector3 nearDownLeft  = nearPlaneCenter - right * nearWidth2 - up * nearHeight2;

    Vector3 farUpLeft     = farPlaneCenter - right * farWidth2 + up * farHeight2;
    Vector3 farUpRight    = farPlaneCenter + right * farWidth2 + up * farHeight2;
    Vector3 farDownRight  = farPlaneCenter + right * farWidth2 - up * farHeight2;
    Vector3 farDownLeft   = farPlaneCenter - right * farWidth2 - up * farHeight2;

    Gizmos::SetResetAllowed(false);

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

    Gizmos::SetResetAllowed(true);

    Gizmos::Reset();
}

void Gizmos::RenderSimpleSphere(const Vector3 &origin, float radius)
{
    ASSERT(Gizmos::GetGizmosGameObject());

    EditorGameObject *gizmosGo = Gizmos::GetGizmosGameObject();
    CircleRenderer *cr = gizmosGo->GetComponent<CircleRenderer>();
    cr->SetEnabled(true);
    cr->SetRadius(radius);

    gizmosGo->transform->SetPosition(origin);

    gizmosGo->transform->SetLocalEuler(0, 0, 0);
    Gizmos::Render(cr);
    gizmosGo->transform->SetLocalEuler(0, 90, 0);
    Gizmos::Render(cr);

    gizmosGo->transform->SetLocalEuler(0, 0, 0);
    Gizmos::Render(cr);
    gizmosGo->transform->SetLocalEuler(90, 0, 0);
    Gizmos::Render(cr);

    Gizmos::Reset();
}

void Gizmos::Reset()
{
    Gizmos::Init();

    if (!m_resetAllowed) { return; }

    Gizmos::SetPosition(Vector3::Zero);
    Gizmos::SetRotation(Quaternion::Identity);
    Gizmos::SetScale(Vector3::One);
    Gizmos::SetColor(Color::Green);
    Gizmos::SetLineWidth(1.0f);
    Gizmos::SetReceivesLighting(false);
    Gizmos::SetDrawWireframe(false);

    for (Renderer *rend : m_renderers)
    {
        rend->SetEnabled(false);
    }

    EditorGameObject *gizmosGo = Gizmos::GetGizmosGameObject();
    MeshRenderer *mr = gizmosGo->GetComponent<MeshRenderer>();
    mr->GetMaterial()->SetTexture(nullptr);
}

void Gizmos::Render(Renderer *rend)
{
    for (Renderer *r : m_renderers)
    {
        r->SetEnabled(r == rend); // Enable only rend
    }

    GraphicPipeline *gp = GraphicPipeline::GetActive(); ASSERT(gp);
    gp->RenderRenderer(rend);
}
