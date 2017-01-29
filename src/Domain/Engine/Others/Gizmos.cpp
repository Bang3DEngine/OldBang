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
#include "EditorGizmosGameObject.h"

void Gizmos::SetResetAllowed(bool allowed)
{
    EditorGizmosGameObject::GetInstance()->SetResetAllowed(allowed);
}

void Gizmos::SetColor(const Color &color)
{
    EditorGizmosGameObject::GetInstance()->SetColor(color);
}

void Gizmos::SetPosition(const Vector3 &position)
{
    EditorGizmosGameObject::GetInstance()->SetPosition(position);
}

void Gizmos::SetRotation(const Quaternion &rotation)
{
    EditorGizmosGameObject::GetInstance()->SetRotation(rotation);
}

void Gizmos::SetScale(const Vector3 &scale)
{
    EditorGizmosGameObject::GetInstance()->SetScale(scale);
}

void Gizmos::SetLineWidth(float lineWidth)
{
    EditorGizmosGameObject::GetInstance()->SetLineWidth(lineWidth);
}

void Gizmos::SetDrawWireframe(bool wireframe)
{
    EditorGizmosGameObject::GetInstance()->
            SetDrawWireframe(wireframe);
}

void Gizmos::SetReceivesLighting(bool receivesLighting)
{
    EditorGizmosGameObject::GetInstance()->
            SetReceivesLighting(receivesLighting);
}

void Gizmos::SetBillboard()
{
    EditorGizmosGameObject::GetInstance()->SetBillboard();
}

void Gizmos::RenderCustomMesh(Mesh *m)
{
    EditorGizmosGameObject::GetInstance()->
            RenderCustomMesh(m);
}


void Gizmos::RenderSimpleBox(const AABox &b)
{
    EditorGizmosGameObject::GetInstance()->
            RenderSimpleBox(b);
}

void Gizmos::RenderBox(const AABox &b)
{
    EditorGizmosGameObject::GetInstance()->
            RenderBox(b);
}

void Gizmos::RenderRect(const Rect &r)
{
    EditorGizmosGameObject::GetInstance()->
            RenderRect(r);
}

void Gizmos::RenderIcon(const Texture2D *texture,
                        bool billboard)
{
    EditorGizmosGameObject::GetInstance()->
            RenderIcon(texture, billboard);
}

void Gizmos::RenderCircle(float radius)
{
    EditorGizmosGameObject::GetInstance()->
            RenderCircle(radius);
}

void Gizmos::RenderLine(const Vector3 &origin, const Vector3 &destiny)
{
    EditorGizmosGameObject::GetInstance()->
            RenderLine(origin, destiny);
}

void Gizmos::RenderScreenLine(const Vector2 &origin, const Vector2 &destiny)
{
    EditorGizmosGameObject::GetInstance()->
            RenderScreenLine(origin, destiny);
}

void Gizmos::RenderRay(const Vector3 &origin, const Vector3 &rayDir)
{
    EditorGizmosGameObject::GetInstance()->
            RenderRay(origin, rayDir);
}

void Gizmos::RenderSphere(const Vector3 &origin, float radius)
{
    EditorGizmosGameObject::GetInstance()->
            RenderSphere(origin, radius);
}

void Gizmos::RenderFrustum(const Vector3 &forward, const Vector3 &up,
                           const Vector3 &origin,
                           float zNear, float zFar,
                           float fovDegrees, float aspectRatio)
{
    EditorGizmosGameObject::GetInstance()->
            RenderFrustum(forward, up, origin, zNear, zFar,
                          fovDegrees, aspectRatio);
}

void Gizmos::RenderSimpleSphere(const Vector3 &origin, float radius)
{
    EditorGizmosGameObject::GetInstance()->
            RenderSimpleSphere(origin, radius);
}

void Gizmos::Reset()
{
    EditorGizmosGameObject::GetInstance()->Reset();
}

void Gizmos::Render(Renderer *rend)
{
    EditorGizmosGameObject::GetInstance()->Render(rend);
}
