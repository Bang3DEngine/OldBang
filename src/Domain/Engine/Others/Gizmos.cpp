#include "Gizmos.h"

#include "Material.h"
#include "Texture2D.h"
#include "MeshRenderer.h"
#include "AssetsManager.h"
#include "CircleRenderer.h"
#include "EditorGameObject.h"
#include "SingleLineRenderer.h"

EditorGameObject *Gizmos::m_gizmosGameObject = nullptr;

SingleLineRenderer *Gizmos::m_singleLineRenderer = nullptr;
CircleRenderer *Gizmos::m_circleRenderer = nullptr;
MeshRenderer *Gizmos::m_meshRenderer = nullptr;
Mesh *Gizmos::m_boxMesh = nullptr;
Mesh *Gizmos::m_sphereMesh = nullptr;
Mesh *Gizmos::m_planeMesh = nullptr;
bool Gizmos::m_wireframe = false;

Material *Gizmos::m_material = nullptr;

Color Gizmos::m_color = Color::white;
float Gizmos::m_lineWidth = 1.0f;
bool Gizmos::m_receivesLighting = false;

bool Gizmos::m_inited = false;

void Gizmos::Init()
{
    if (!Gizmos::m_inited)
    {
        Gizmos::m_boxMesh = MeshFactory::GetCube();
        Gizmos::m_sphereMesh = MeshFactory::GetSphere();
        Gizmos::m_planeMesh = MeshFactory::GetPlane();
        Gizmos::m_material = AssetsManager::LoadAsset<Material>("./Assets/Engine/Materials/D2G_Default.bmat");

        Gizmos::m_inited = true;
    }
}

void Gizmos::SetGizmosGameObject(EditorGameObject *ego)
{
    Init();

    if (m_gizmosGameObject)
    {
        delete m_gizmosGameObject;
    }
    m_gizmosGameObject = ego;

    Gizmos::m_singleLineRenderer = m_gizmosGameObject->AddComponent<SingleLineRenderer>();
    Gizmos::m_singleLineRenderer->SetMaterial(Gizmos::m_material);

    Gizmos::m_circleRenderer = m_gizmosGameObject->AddComponent<CircleRenderer>();
    Gizmos::m_circleRenderer->SetMaterial(Gizmos::m_material);

    Gizmos::m_meshRenderer = m_gizmosGameObject->AddComponent<MeshRenderer>();
    Gizmos::m_meshRenderer->SetMaterial(Gizmos::m_material);

}

void Gizmos::SetStatesBeforeDrawing()
{
    Gizmos::m_singleLineRenderer->GetMaterial()->SetDiffuseColor(m_color);
    Gizmos::m_circleRenderer->GetMaterial()->SetDiffuseColor(m_color);
    Gizmos::m_meshRenderer->GetMaterial()->SetDiffuseColor(m_color);

    Gizmos::m_singleLineRenderer->SetLineWidth(m_lineWidth);
    Gizmos::m_circleRenderer->SetLineWidth(m_lineWidth);
    Gizmos::m_meshRenderer->SetLineWidth(m_lineWidth);

    Gizmos::m_singleLineRenderer->SetReceivesLighting(m_receivesLighting);
    Gizmos::m_circleRenderer->SetReceivesLighting(m_receivesLighting);
    Gizmos::m_meshRenderer->SetReceivesLighting(m_receivesLighting);

    Gizmos::m_singleLineRenderer->SetDrawWireframe(m_wireframe);
    Gizmos::m_circleRenderer->SetDrawWireframe(m_wireframe);
    Gizmos::m_meshRenderer->SetDrawWireframe(m_wireframe);

    Gizmos::m_meshRenderer->GetMaterial()->SetTexture(nullptr);

    Gizmos::m_gizmosGameObject->transform->SetPosition(Vector3::zero);
    Gizmos::m_gizmosGameObject->transform->SetRotation(Quaternion::identity);
    Gizmos::m_gizmosGameObject->transform->SetScale(Vector3::one);
}

void Gizmos::OnNewFrame()
{
    Gizmos::m_singleLineRenderer->SetEnabled(false);
    Gizmos::m_circleRenderer->SetEnabled(false);
    Gizmos::m_meshRenderer->SetEnabled(false);
}

void Gizmos::SetColor(const Color &color)
{
    Gizmos::m_color = color;
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


void Gizmos::DrawSimpleBox(const Box &b, const Quaternion& rotation)
{
    Gizmos::Init();
    Gizmos::SetStatesBeforeDrawing();

    Quaternion r = rotation;
    Vector3 bMin = b.GetMin();
    Vector3 bMax = b.GetMax();
    Gizmos::DrawLine(r * Vector3(bMin.x, bMin.y, bMin.z), r * Vector3(bMax.x, bMin.y, bMin.z));
    Gizmos::DrawLine(r * Vector3(bMin.x, bMin.y, bMin.z), r * Vector3(bMin.x, bMax.y, bMin.z));
    Gizmos::DrawLine(r * Vector3(bMin.x, bMin.y, bMin.z), r * Vector3(bMin.x, bMin.y, bMax.z));

    Gizmos::DrawLine(r * Vector3(bMax.x, bMin.y, bMin.z), r * Vector3(bMax.x, bMax.y, bMin.z));
    Gizmos::DrawLine(r * Vector3(bMax.x, bMin.y, bMin.z), r * Vector3(bMax.x, bMin.y, bMax.z));

    Gizmos::DrawLine(r * Vector3(bMin.x, bMax.y, bMin.z), r * Vector3(bMax.x, bMax.y, bMin.z));
    Gizmos::DrawLine(r * Vector3(bMin.x, bMax.y, bMin.z), r * Vector3(bMin.x, bMax.y, bMax.z));

    Gizmos::DrawLine(r * Vector3(bMin.x, bMin.y, bMax.z), r * Vector3(bMax.x, bMin.y, bMax.z));
    Gizmos::DrawLine(r * Vector3(bMin.x, bMin.y, bMax.z), r * Vector3(bMin.x, bMax.y, bMax.z));

    Gizmos::DrawLine(r * Vector3(bMin.x, bMax.y, bMax.z), r * Vector3(bMax.x, bMax.y, bMax.z));
    Gizmos::DrawLine(r * Vector3(bMax.x, bMin.y, bMax.z), r * Vector3(bMax.x, bMax.y, bMax.z));
    Gizmos::DrawLine(r * Vector3(bMax.x, bMax.y, bMin.z), r * Vector3(bMax.x, bMax.y, bMax.z));
}

void Gizmos::DrawBox(const Box &b, const Quaternion& rotation)
{
    Gizmos::Init();
    Gizmos::SetStatesBeforeDrawing();

    Gizmos::m_meshRenderer->SetEnabled(true);
    Gizmos::m_meshRenderer->SetMesh(Gizmos::m_boxMesh);

    Gizmos::m_gizmosGameObject->transform->SetPosition(b.GetCenter());
    Gizmos::m_gizmosGameObject->transform->SetRotation(rotation);
    Gizmos::m_gizmosGameObject->transform->SetScale(b.GetDimensions());

    Gizmos::m_meshRenderer->Render();
}

void Gizmos::DrawIcon(const Texture2D *texture,
                      const Vector3 &position, const Vector3 &scale,
                      bool billboard)
{
    Gizmos::Init();
    Gizmos::SetReceivesLighting(false);
    Gizmos::SetDrawWireframe(false);
    Gizmos::SetStatesBeforeDrawing();

    Gizmos::m_meshRenderer->SetEnabled(true);
    Gizmos::m_meshRenderer->SetMesh(Gizmos::m_planeMesh);

    Gizmos::m_gizmosGameObject->transform->SetPosition(position);
    Gizmos::m_gizmosGameObject->transform->SetScale(scale);
    if (billboard)
    {
        Scene *scene = Scene::GetCurrentScene();
        Transform *camTransform = scene->GetCamera()->transform;
        Gizmos::m_gizmosGameObject->
                transform->LookInDirection(camTransform->GetForward(),
                                           camTransform->GetUp());
    }

    Gizmos::m_meshRenderer->GetMaterial()->SetTexture(texture);

    Gizmos::m_meshRenderer->Render();
}

void Gizmos::DrawLine(const Vector3 &origin, const Vector3 &destiny)
{
    Gizmos::Init();
    Gizmos::SetStatesBeforeDrawing();

    Gizmos::m_singleLineRenderer->SetEnabled(true);
    Gizmos::m_singleLineRenderer->SetOrigin(origin);
    Gizmos::m_singleLineRenderer->SetDestiny(destiny);

    Gizmos::m_gizmosGameObject->transform->SetPosition(Vector3::zero);
    Gizmos::m_gizmosGameObject->transform->SetScale(Vector3::one);

    Gizmos::m_singleLineRenderer->Render();
}

void Gizmos::DrawRay(const Vector3 &origin, const Vector3 &rayDir)
{
    Gizmos::DrawLine(origin, origin + rayDir);
}

void Gizmos::DrawSphere(const Vector3 &origin, float radius)
{
    Gizmos::Init();
    Gizmos::SetStatesBeforeDrawing();

    Gizmos::m_meshRenderer->SetEnabled(true);
    Gizmos::m_meshRenderer->SetMesh(Gizmos::m_sphereMesh);

    Gizmos::m_gizmosGameObject->transform->SetPosition(origin);
    Gizmos::m_gizmosGameObject->transform->SetScale(radius);

    Gizmos::m_meshRenderer->Render();
}

void Gizmos::DrawFrustum(const Vector3 &forward, const Vector3 &up,
                         const Vector3 &origin,
                         float zNear, float zFar,
                         float fovDegrees, float aspectRatio)
{
    Gizmos::Init();
    Gizmos::SetStatesBeforeDrawing();

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
    Gizmos::DrawLine(nearUpLeft   , nearUpRight);
    Gizmos::DrawLine(nearUpRight  , nearDownRight);
    Gizmos::DrawLine(nearDownRight, nearDownLeft);
    Gizmos::DrawLine(nearDownLeft , nearUpLeft);

    // Far plane
    Gizmos::DrawLine(farUpLeft   , farUpRight);
    Gizmos::DrawLine(farUpRight  , farDownRight);
    Gizmos::DrawLine(farDownRight, farDownLeft);
    Gizmos::DrawLine(farDownLeft , farUpLeft);

    // Projection lines
    Gizmos::DrawLine(nearUpLeft   , farUpLeft);
    Gizmos::DrawLine(nearUpRight  , farUpRight);
    Gizmos::DrawLine(nearDownRight, farDownRight);
    Gizmos::DrawLine(nearDownLeft , farDownLeft);
}

void Gizmos::DrawSimpleSphere(const Vector3 &origin, float radius)
{
    Gizmos::Init();
    Gizmos::SetStatesBeforeDrawing();

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
}
