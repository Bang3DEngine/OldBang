#include "Gizmos.h"

#include "Rect.h"
#include "Debug.h"
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
#include "SingleLineRenderer.h"

#ifdef BANG_EDITOR
#include "EditorScene.h"
#include "EditorGameObject.h"
#include "EditorGizmosGameObject.h"
#else
class EditorGizmosGameObject {};
#endif

void Gizmos::SetResetAllowed(bool allowed)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->SetResetAllowed(allowed);
#endif
}

void Gizmos::SetColor(const Color &color)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->SetColor(color);
#endif
}

void Gizmos::SetPosition(const Vector3 &position)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->SetPosition(position);
#endif
}

void Gizmos::SetRotation(const Quaternion &rotation)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->SetRotation(rotation);
#endif
}

void Gizmos::SetScale(const Vector3 &scale)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->SetScale(scale);
#endif
}

void Gizmos::SetLineWidth(float lineWidth)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->SetLineWidth(lineWidth);
#endif
}

void Gizmos::SetDrawWireframe(bool wireframe)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            SetDrawWireframe(wireframe);
#endif
}

void Gizmos::SetReceivesLighting(bool receivesLighting)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            SetReceivesLighting(receivesLighting);
#endif
}

void Gizmos::SetBillboard()
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->SetBillboard();
#endif
}

void Gizmos::SetScreenSpaceMode()
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->SetScreenSpaceMode(true);
#endif
}

void Gizmos::RenderCustomMesh(Mesh *m)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            RenderCustomMesh(m);
#endif
}


void Gizmos::RenderSimpleBox(const AABox &b)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            RenderSimpleBox(b);
#endif
}

void Gizmos::RenderBox(const AABox &b)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            RenderBox(b);
#endif
}

void Gizmos::RenderRect(const Rect &r)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            RenderRect(r);
#endif
}

void Gizmos::RenderIcon(const Texture2D *texture,
                        bool billboard)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            RenderIcon(texture, billboard);
#endif
}

void Gizmos::RenderScreenIcon(const Texture2D *texture,
                              const Rect &screenRect)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            RenderScreenIcon(texture, screenRect);
#endif
}

void Gizmos::RenderCircle(float radius)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            RenderCircle(radius);
#endif
}

void Gizmos::RenderLine(const Vector3 &origin, const Vector3 &destiny)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            RenderLine(origin, destiny);
#endif
}

void Gizmos::RenderScreenLine(const Vector2 &origin, const Vector2 &destiny)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            RenderScreenLine(origin, destiny);
#endif
}

void Gizmos::RenderRay(const Vector3 &origin, const Vector3 &rayDir)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            RenderRay(origin, rayDir);
#endif
}

void Gizmos::RenderSphere(const Vector3 &origin, float radius)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            RenderSphere(origin, radius);
#endif
}

void Gizmos::RenderFrustum(const Vector3 &forward, const Vector3 &up,
                           const Vector3 &origin,
                           float zNear, float zFar,
                           float fovDegrees, float aspectRatio)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            RenderFrustum(forward, up, origin, zNear, zFar,
                          fovDegrees, aspectRatio);
#endif
}

void Gizmos::RenderSimpleSphere(const Vector3 &origin, float radius)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->
            RenderSimpleSphere(origin, radius);
#endif
}

EditorGizmosGameObject *Gizmos::GetEditorGizmosGameObject()
{
    #ifdef BANG_EDITOR
    return EditorGizmosGameObject::GetInstance();
    #else
    return nullptr;
    #endif
}

void Gizmos::Reset()
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->Reset();
#endif
}

void Gizmos::Render(Renderer *rend)
{
#ifdef BANG_EDITOR
    Gizmos::GetEditorGizmosGameObject()->Render(rend);
#endif
}
