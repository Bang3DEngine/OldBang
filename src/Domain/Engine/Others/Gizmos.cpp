#include "Gizmos.h"

#include "Renderer.h"
#include "Material.h"
#include "Texture2D.h"
#include "MeshRenderer.h"
#include "AssetsManager.h"
#include "CircleRenderer.h"
#include "EditorGameObject.h"
#include "SingleLineRenderer.h"

EditorGameObject *Gizmos::m_gizmosGameObject = nullptr;
std::vector<Renderer*> Gizmos::renderers;

SingleLineRenderer *Gizmos::m_singleLineRenderer = nullptr;
CircleRenderer *Gizmos::m_circleRenderer = nullptr;
MeshRenderer *Gizmos::m_meshRenderer = nullptr;
Mesh *Gizmos::m_boxMesh = nullptr;
Mesh *Gizmos::m_sphereMesh = nullptr;
Mesh *Gizmos::m_planeMesh = nullptr;
bool  Gizmos::m_wireframe = false;

Material *Gizmos::m_material = nullptr;

Color Gizmos::m_color = Color::white;
Vector3 Gizmos::m_position = Vector3::zero;
Quaternion Gizmos::m_rotation = Quaternion::identity;
Vector3 Gizmos::m_scale = Vector3::one;
float Gizmos::m_lineWidth = 1.0f;
bool  Gizmos::m_ignoreModel      = false;
bool  Gizmos::m_ignoreView       = false;
bool  Gizmos::m_ignoreProjection = false;
bool  Gizmos::m_receivesLighting = false;

bool Gizmos::m_inited = false;

void Gizmos::Init()
{
    if (!Gizmos::m_inited)
    {
        Gizmos::m_boxMesh = MeshFactory::GetCube();
        Gizmos::m_sphereMesh = MeshFactory::GetSphere();
        Gizmos::m_planeMesh = MeshFactory::GetPlane();
        Gizmos::m_material = AssetsManager::LoadAsset<Material>("./Assets/Engine/Materials/D2G_Default.bmat");
        Gizmos::m_material = new Material(*Gizmos::m_material);
        Gizmos::m_inited = true;
    }
}

void Gizmos::SetGizmosGameObject(EditorGameObject *ego)
{
    Gizmos::Init();

    m_gizmosGameObject = ego; // Do not delete last gizmosGameObject, scene delete will do so

    Gizmos::m_singleLineRenderer = m_gizmosGameObject->AddComponent<SingleLineRenderer>();
    Gizmos::m_circleRenderer = m_gizmosGameObject->AddComponent<CircleRenderer>();
    Gizmos::m_meshRenderer = m_gizmosGameObject->AddComponent<MeshRenderer>();

    renderers.clear();
    renderers.push_back(Gizmos::m_singleLineRenderer);
    renderers.push_back(Gizmos::m_circleRenderer);
    renderers.push_back(Gizmos::m_meshRenderer);

    for (Renderer *rend : Gizmos::renderers)
    {
        rend->SetMaterial(Gizmos::m_material);
    }
}

void Gizmos::SetStatesBeforeRendering()
{
    Gizmos::Init();

    Gizmos::m_gizmosGameObject->transform->SetLocalPosition(Gizmos::m_position);
    Gizmos::m_gizmosGameObject->transform->SetLocalRotation(Gizmos::m_rotation);
    Gizmos::m_gizmosGameObject->transform->SetLocalScale(Gizmos::m_scale);

    for (Renderer *rend : Gizmos::renderers)
    {
        rend->GetMaterial()->SetDiffuseColor(m_color);
        rend->SetLineWidth(m_lineWidth);
        rend->SetReceivesLighting(m_receivesLighting);
        rend->SetDrawWireframe(m_wireframe);
        rend->SetIgnoreModelMatrix(m_ignoreModel);
        rend->SetIgnoreViewMatrix(m_ignoreView);
        rend->SetIgnoreProjectionMatrix(m_ignoreProjection);
        /*
        rend->transform->SetPosition(Vector3::zero);
        rend->transform->SetRotation(Quaternion::identity);
        rend->transform->SetScale(Vector3::one);
        */
    }
    Gizmos::m_meshRenderer->GetMaterial()->SetTexture(nullptr);
}

void Gizmos::SetColor(const Color &color)
{
    Gizmos::m_color = color;
}

void Gizmos::SetPosition(const Vector3 &position)
{
    Gizmos::m_position = position;
}

void Gizmos::SetRotation(const Quaternion &rotation)
{
    Gizmos::m_rotation = rotation;
}

void Gizmos::SetScale(const Vector3 &scale)
{
    Gizmos::m_scale = scale;
}

void Gizmos::SetLineWidth(float lineWidth)
{
    Gizmos::m_lineWidth = lineWidth;
}

void Gizmos::SetDrawWireframe(bool wireframe)
{
    m_wireframe = wireframe;
}

void Gizmos::SetReceivesLighting(bool receivesLighting)
{
    Gizmos::m_receivesLighting = receivesLighting;
}

void Gizmos::SetIgnoreMatrices(bool ignoreModel, bool ignoreView,
                               bool ignoreProjection)
{
    m_ignoreModel = ignoreModel;
    m_ignoreView = ignoreView;
    m_ignoreProjection = ignoreProjection;
}

void Gizmos::RenderCustomMesh(Mesh *m)
{
    NONULL(m)

    Gizmos::SetStatesBeforeRendering();

    Gizmos::m_meshRenderer->SetEnabled(true);
    Gizmos::m_meshRenderer->SetMesh(m);
    Gizmos::m_meshRenderer->Render();

    Gizmos::Reset();
}


void Gizmos::RenderSimpleBox(const Box &b)
{
    Gizmos::SetStatesBeforeRendering();

    const Quaternion &r = Gizmos::m_rotation;
    const Vector3 &bMin = b.GetMin();
    const Vector3& bMax = b.GetMax();
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

    Gizmos::Reset();
}

void Gizmos::RenderBox(const Box &b)
{
    Gizmos::SetStatesBeforeRendering();

    Gizmos::m_meshRenderer->SetEnabled(true);
    Gizmos::m_meshRenderer->SetMesh(Gizmos::m_boxMesh);

    Gizmos::m_gizmosGameObject->transform->SetPosition(b.GetCenter());
    Gizmos::m_gizmosGameObject->transform->SetScale(b.GetDimensions());

    Gizmos::m_meshRenderer->Render();

    Gizmos::Reset();
}

void Gizmos::RenderIcon(const Texture2D *texture,
                        bool billboard)
{
    Gizmos::SetReceivesLighting(false);
    Gizmos::SetDrawWireframe(false);
    Gizmos::SetStatesBeforeRendering();

    Gizmos::m_meshRenderer->SetEnabled(true);
    Gizmos::m_meshRenderer->SetMesh(Gizmos::m_planeMesh);

    if (billboard)
    {
        Camera *cam = Scene::GetCamera();
        Vector3 camPos = cam->transform->GetPosition();

        float distScale = 1.0f;
        if (cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
        {
           distScale = Vector3::Distance(camPos, Gizmos::m_position);
        }

        Gizmos::m_gizmosGameObject-> transform->SetScale(m_scale * distScale);
        Gizmos::m_gizmosGameObject->
                transform->LookInDirection(cam->transform->GetForward(),
                                           cam->transform->GetUp());
    }

    Gizmos::m_meshRenderer->GetMaterial()->SetTexture(texture);

    Gizmos::m_meshRenderer->Render();

    Gizmos::Reset();
}

void Gizmos::RenderLine(const Vector3 &origin, const Vector3 &destiny)
{
    Gizmos::SetStatesBeforeRendering();

    Gizmos::m_singleLineRenderer->SetEnabled(true);
    Gizmos::m_singleLineRenderer->SetOrigin(origin);
    Gizmos::m_singleLineRenderer->SetDestiny(destiny);

    Gizmos::m_gizmosGameObject->transform->SetPosition(Vector3::zero);
    Gizmos::m_gizmosGameObject->transform->SetScale(Vector3::one);

    Gizmos::m_singleLineRenderer->Render();

    Gizmos::Reset();
}

void Gizmos::RenderRay(const Vector3 &origin, const Vector3 &rayDir)
{
    Gizmos::RenderLine(origin, origin + rayDir);

    Gizmos::Reset();
}

void Gizmos::RenderSphere(const Vector3 &origin, float radius)
{
    Gizmos::SetStatesBeforeRendering();

    Gizmos::m_meshRenderer->SetEnabled(true);
    Gizmos::m_meshRenderer->SetMesh(Gizmos::m_sphereMesh);

    Gizmos::m_gizmosGameObject->transform->SetPosition(origin);
    Gizmos::m_gizmosGameObject->transform->SetScale(radius);

    Gizmos::m_meshRenderer->Render();

    Gizmos::Reset();
}

void Gizmos::RenderFrustum(const Vector3 &forward, const Vector3 &up,
                           const Vector3 &origin,
                           float zNear, float zFar,
                           float fovDegrees, float aspectRatio)
{
    Gizmos::SetStatesBeforeRendering();

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

    Gizmos::Reset();
}

void Gizmos::RenderSimpleSphere(const Vector3 &origin, float radius)
{
    Gizmos::SetStatesBeforeRendering();

    Gizmos::m_circleRenderer->SetEnabled(true);
    Gizmos::m_circleRenderer->SetRadius(radius);

    Gizmos::m_gizmosGameObject->transform->SetPosition(origin);

    Gizmos::m_gizmosGameObject->transform->SetLocalEuler(0, 0, 0);
    Gizmos::m_circleRenderer->Render();
    Gizmos::m_gizmosGameObject->transform->SetLocalEuler(0, 90, 0);
    Gizmos::m_circleRenderer->Render();

    Gizmos::m_gizmosGameObject->transform->SetLocalEuler(0, 0, 0);
    Gizmos::m_circleRenderer->Render();
    Gizmos::m_gizmosGameObject->transform->SetLocalEuler(90, 0, 0);
    Gizmos::m_circleRenderer->Render();

    Gizmos::Reset();
}

void Gizmos::Reset()
{
    Gizmos::Init();

    Gizmos::m_position = Vector3::zero;
    Gizmos::m_rotation = Quaternion::identity;
    Gizmos::m_scale = Vector3::one;
    for (Renderer *rend : Gizmos::renderers)
    {
        rend->GetMaterial()->SetDiffuseColor(Color::green);
        rend->SetLineWidth(1.0f);
        rend->SetReceivesLighting(false);
        rend->SetDrawWireframe(true);
        rend->SetIgnoreModelMatrix(false);
        rend->SetIgnoreViewMatrix(false);
        rend->SetIgnoreProjectionMatrix(false);
        /*
        rend->transform->SetPosition(Vector3::zero);
        rend->transform->SetRotation(Quaternion::identity);
        rend->transform->SetScale(Vector3::one);
        */
        rend->SetEnabled(false);
    }

    Gizmos::m_meshRenderer->GetMaterial()->SetTexture(nullptr);
}
