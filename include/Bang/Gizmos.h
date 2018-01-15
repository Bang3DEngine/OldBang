#ifndef GIZMOS_H
#define GIZMOS_H

#include "Bang/GameObject.h"
#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

FORWARD   class Material;
FORWARD   class Renderer;
FORWARD   class Texture2D;
FORWARD   class LineRenderer;
FORWARD   class MeshRenderer;

class Gizmos
{
public:
    static void SetColor(const Color &color);

    static void SetPosition(const Vector3 &position);
    static void SetRotation(const Quaternion &rotation);
    static void SetScale(const Vector3 &scale);

    static void SetRenderPass(RenderPass rp);
    static void SetSelectable(GameObject *go);
    static void SetThickness(float thickness);
    static void SetRenderWireframe(bool wireframe);
    static void SetReceivesLighting(bool receivesLighting);
    static void SetBillboard();
    static void RenderCustomMesh(Mesh *m);
    static void RenderBox(const AABox &b);
    static void RenderSimpleBox(const AABox &b);
    static void RenderRect(const Rect &r);
    static void RenderFillRect(const Rect &r);
    static void RenderIcon(Texture2D *texture,
                           bool billboard = true);
    static void RenderScreenIcon(Texture2D *texture,
                                 const Rect &screenRect,
                                 bool fixAspectRatio = false);
    static void RenderScreenLine(const Vector2 &origin,
                                 const Vector2 &destiny);
    static void RenderLine(const Vector3 &origin,
                           const Vector3 &destiny);
    static void RenderRay(const Vector3 &origin,
                          const Vector3 &rayDir);
    static void RenderSphere(const Vector3 &origin, float radius);
    static void RenderFrustum(const Vector3 &forward,
                              const Vector3 &up,
                              const Vector3 &origin,
                              float zNear, float zFar,
                              float fovDegrees, float aspectRatio);
    static void RenderPoint(const Vector3 &point);

    static void Render(Renderer *rend);

    static void Reset();

    GameObject *GetGameObject() const;

private:
    GameObject *m_gizmosGo = nullptr;

    RH<Mesh> p_boxMesh;
    RH<Mesh> p_planeMesh;
    RH<Mesh> p_sphereMesh;

    GameObject *p_selectable = nullptr;
    LineRenderer *m_lineRenderer = nullptr;
    MeshRenderer *m_meshRenderer = nullptr;
    List<Renderer*> m_renderers;

    Gizmos();
    virtual ~Gizmos();

    static GameObject *GetGizmosGameObject();
    static Gizmos *GetInstance();

    friend class Scene;
};

NAMESPACE_BANG_END

#endif // GIZMOS_H
