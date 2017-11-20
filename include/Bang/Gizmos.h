#ifndef GIZMOS_H
#define GIZMOS_H

#include "Bang/GameObject.h"

NAMESPACE_BANG_BEGIN

FORWARD   class Material;
FORWARD   class Renderer;
FORWARD   class Texture2D;
FORWARD   class MeshRenderer;
FORWARD   class CircleRenderer;
FORWARD   class SingleLineRenderer;

class Gizmos
{
public:
    static void SetColor(const Color &color);

    static void SetPosition(const Vector3 &position);
    static void SetRotation(const Quaternion &rotation);
    static void SetScale(const Vector3 &scale);

    static void SetLineWidth(float lineWidth);
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
    static void RenderCircle(float radius);
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
    static void RenderSimpleSphere(const Vector3 &origin, float radius);

    static void Render(Renderer *rend);

    static void Reset();

private:
    GameObject *m_gizmosGo = nullptr;

    Mesh *m_boxMesh      = nullptr;
    Mesh *m_sphereMesh   = nullptr;
    Mesh *m_planeMesh    = nullptr;
    Material *m_material = nullptr;

    SingleLineRenderer *m_singleLineRenderer = nullptr;
    CircleRenderer *m_circleRenderer = nullptr;
    MeshRenderer *m_meshRenderer = nullptr;
    List<Renderer*> m_renderers;

    Gizmos();

    static GameObject *GetGizmosGameObject();
    static Gizmos *GetInstance();

    friend class Scene;
};

NAMESPACE_BANG_END

#endif // GIZMOS_H
