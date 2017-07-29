#ifndef GIZMOS_H
#define GIZMOS_H

#include "Bang/GameObject.h"

FORWARD   class Mesh;
FORWARD   class AABox;
FORWARD   class Color;
FORWARD_T class RectG;
FORWARD_T class Vector2G;
FORWARD_T class Vector3G;
FORWARD_T class Vector4G;
FORWARD   class Material;
FORWARD   class Renderer;
FORWARD   class Transform;
FORWARD   class Texture2D;
FORWARD_T class QuaternionG;
FORWARD   class MeshRenderer;
FORWARD   class CircleRenderer;
FORWARD   class SingleLineRenderer;

class Gizmos : public GameObject
{
public:
    static void SetColor(const Color &color);

    static void SetPosition(const Vector3G<float> &position);
    static void SetRotation(const QuaternionG<float> &rotation);
    static void SetScale(const Vector3G<float> &scale);

    static void SetLineWidth(float lineWidth);
    static void SetDrawWireframe(bool wireframe);
    static void SetReceivesLighting(bool receivesLighting);
    static void SetBillboard();
    static void RenderCustomMesh(Mesh *m);
    static void RenderBox(const AABox &b);
    static void RenderSimpleBox(const AABox &b);
    static void RenderRect(const RectG<float> &r);
    static void RenderFillRect(const RectG<float> &r);
    static void RenderIcon(const Texture2D *texture,
                           bool billboard = true);
    static void RenderScreenIcon(const Texture2D *texture,
                                 const RectG<float> &screenRect,
                                 bool fixAspectRatio = false);
    static void RenderCircle(float radius);
    static void RenderScreenLine(const Vector2G<float> &origin,
                                 const Vector2G<float> &destiny);
    static void RenderLine(const Vector3G<float> &origin,
                           const Vector3G<float> &destiny);
    static void RenderRay(const Vector3G<float> &origin,
                          const Vector3G<float> &rayDir);
    static void RenderSphere(const Vector3G<float> &origin, float radius);
    static void RenderFrustum(const Vector3G<float> &forward,
                              const Vector3G<float> &up,
                              const Vector3G<float> &origin,
                              float zNear, float zFar,
                              float fovDegrees, float aspectRatio);
    static void RenderSimpleSphere(const Vector3G<float> &origin,
                                   float radius);

    static void Render(Renderer *rend);

private:
    Mesh *m_boxMesh      = nullptr;
    Mesh *m_sphereMesh   = nullptr;
    Mesh *m_planeMesh    = nullptr;
    Material *m_material = nullptr;

    SingleLineRenderer *m_singleLineRenderer = nullptr;
    CircleRenderer *m_circleRenderer = nullptr;
    MeshRenderer *m_meshRenderer = nullptr;
    List<Renderer*> m_renderers;

    bool m_resetAllowed  = true;

    Gizmos();

    void SetResetAllowed(bool allowed);
    void Reset();

    static Gizmos *GetInstance();

    friend class Scene;
};

#endif // GIZMOS_H
