#include "Gizmos.h"

#include "Debug.h"
#include "Rect.h"
#include "Scene.h"
#include "Camera.h"
#include "Renderer.h"
#include "Material.h"
#include "Transform.h"
#include "Texture2D.h"
#include "MeshFactory.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "AssetsManager.h"
#include "CircleRenderer.h"
#include "GraphicPipeline.h"
#include "EditorGameObject.h"
#include "SingleLineRenderer.h"

EditorGameObject *Gizmos::m_gizmosGameObject = nullptr;
Array<Renderer*> Gizmos::m_renderers;

SingleLineRenderer *Gizmos::m_singleLineRenderer = nullptr;
CircleRenderer *Gizmos::m_circleRenderer = nullptr;
MeshRenderer *Gizmos::m_meshRenderer = nullptr;
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

void Gizmos::SetGizmosGameObject(EditorGameObject *ego)
{
    Gizmos::Init();

    if (m_gizmosGameObject != ego && ego)
    {
        m_gizmosGameObject = ego; // Do not delete last gizmosGameObject, scene delete will do so

        Gizmos::m_singleLineRenderer = m_gizmosGameObject->AddComponent<SingleLineRenderer>();
        Gizmos::m_circleRenderer = m_gizmosGameObject->AddComponent<CircleRenderer>();
        Gizmos::m_meshRenderer = m_gizmosGameObject->AddComponent<MeshRenderer>();

        m_renderers.Clear();
        m_renderers.PushBack(Gizmos::m_singleLineRenderer);
        m_renderers.PushBack(Gizmos::m_circleRenderer);
        m_renderers.PushBack(Gizmos::m_meshRenderer);

        for (Renderer *rend : Gizmos::m_renderers)
        {
            rend->SetIsGizmo(true);
            rend->SetMaterial(Gizmos::m_material);
        }
    }
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
    Gizmos::m_gizmosGameObject->transform->SetLocalPosition(position);
}

void Gizmos::SetRotation(const Quaternion &rotation)
{
    Gizmos::m_gizmosGameObject->transform->SetLocalRotation(rotation);
}

void Gizmos::SetScale(const Vector3 &scale)
{
    Gizmos::m_gizmosGameObject->transform->SetLocalScale(scale);
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
    NONULL(m); NONULL(m_gizmosGameObject);

    Gizmos::m_meshRenderer->SetEnabled(true);
    Gizmos::m_meshRenderer->SetMesh(m);
    Gizmos::Render(Gizmos::m_meshRenderer);

    Gizmos::Reset();
}


void Gizmos::RenderSimpleBox(const Box &b)
{
    NONULL(m_gizmosGameObject);

    const Quaternion &r = Gizmos::m_gizmosGameObject->transform->GetLocalRotation();
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

void Gizmos::RenderBox(const Box &b)
{
    NONULL(m_gizmosGameObject);
    Gizmos::m_meshRenderer->SetEnabled(true);
    Gizmos::m_meshRenderer->SetMesh(Gizmos::m_boxMesh);

    Gizmos::m_gizmosGameObject->transform->SetPosition(b.GetCenter());
    Gizmos::m_gizmosGameObject->transform->SetScale(
                Gizmos::m_gizmosGameObject->transform->GetScale() * b.GetDimensions());

    Gizmos::Render(Gizmos::m_meshRenderer);

    Gizmos::Reset();
}

void Gizmos::RenderRect(const Rect &r)
{
    NONULL(m_gizmosGameObject);
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
    NONULL(m_gizmosGameObject);

    Gizmos::SetDrawWireframe(false);
    Gizmos::SetReceivesLighting(false);
    Gizmos::m_meshRenderer->SetMesh(Gizmos::m_planeMesh);

    if (billboard)
    {
        Camera *cam = Scene::GetCamera();
        Vector3 camPos = cam->transform->GetPosition();

        float distScale = 1.0f;
        if (cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
        {
           Vector3 pos = Gizmos::m_gizmosGameObject->transform->GetPosition();
           distScale = Vector3::Distance(camPos, pos);
        }

        Vector3 scale = Gizmos::m_gizmosGameObject->transform->GetScale();
        Gizmos::m_gizmosGameObject->transform->SetScale(distScale * scale);
        Gizmos::m_gizmosGameObject->
                transform->LookInDirection(cam->transform->GetForward(),
                                           cam->transform->GetUp());
    }
    Gizmos::m_meshRenderer->GetMaterial()->SetTexture(texture);
    Gizmos::Render(Gizmos::m_meshRenderer);

    Gizmos::Reset();
}

void Gizmos::RenderLine(const Vector3 &origin, const Vector3 &destiny)
{
    NONULL(m_gizmosGameObject);

    Gizmos::m_singleLineRenderer->SetOrigin(origin);
    Gizmos::m_singleLineRenderer->SetDestiny(destiny);

    Gizmos::m_gizmosGameObject->transform->SetPosition(Vector3::Zero);
    Gizmos::m_gizmosGameObject->transform->SetScale(Vector3::One);

    Gizmos::Render(Gizmos::m_singleLineRenderer);

    Gizmos::Reset();
}

void Gizmos::RenderScreenLine(const Vector2 &origin, const Vector2 &destiny)
{
    NONULL(m_gizmosGameObject);
    Camera *cam = SceneManager::GetActiveScene()->GetCamera(); NONULL(cam);
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
    NONULL(m_gizmosGameObject);

    Gizmos::m_meshRenderer->SetEnabled(true);
    Gizmos::m_meshRenderer->SetMesh(Gizmos::m_sphereMesh);

    Gizmos::m_gizmosGameObject->transform->SetPosition(origin);
    Gizmos::m_gizmosGameObject->transform->SetScale(radius);

    Gizmos::Render(Gizmos::m_meshRenderer);

    Gizmos::Reset();
}

void Gizmos::RenderFrustum(const Vector3 &forward, const Vector3 &up,
                           const Vector3 &origin,
                           float zNear, float zFar,
                           float fovDegrees, float aspectRatio)
{
    NONULL(m_gizmosGameObject);

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
    NONULL(m_gizmosGameObject);

    Gizmos::m_circleRenderer->SetEnabled(true);
    Gizmos::m_circleRenderer->SetRadius(radius);

    Gizmos::m_gizmosGameObject->transform->SetPosition(origin);

    Gizmos::m_gizmosGameObject->transform->SetLocalEuler(0, 0, 0);
    Gizmos::Render(Gizmos::m_circleRenderer);
    Gizmos::m_gizmosGameObject->transform->SetLocalEuler(0, 90, 0);
    Gizmos::Render(Gizmos::m_circleRenderer);

    Gizmos::m_gizmosGameObject->transform->SetLocalEuler(0, 0, 0);
    Gizmos::Render(Gizmos::m_circleRenderer);
    Gizmos::m_gizmosGameObject->transform->SetLocalEuler(90, 0, 0);
    Gizmos::Render(Gizmos::m_circleRenderer);

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

    Gizmos::m_meshRenderer->GetMaterial()->SetTexture(nullptr);
}

void Gizmos::Render(Renderer *rend)
{
    for (Renderer *r : m_renderers)
    {
        r->SetEnabled(r == rend); // Enable only rend
    }

    GraphicPipeline *gp = GraphicPipeline::GetActive(); NONULL(gp);
    rend->SetDepthLayer(gp->GetCurrentDepthLayer());
    gp->RenderRenderer(rend);
}
