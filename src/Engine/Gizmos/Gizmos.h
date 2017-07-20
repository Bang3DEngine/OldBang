#ifndef GIZMOS_H
#define GIZMOS_H

#include "Bang/AABox.h"
#include "Bang/Color.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Quaternion.h"

class Mesh;
class Rect;
class Material;
class Renderer;
class Transform;
class Texture2D;
class GameObject;
class MeshRenderer;
class CircleRenderer;
class SingleLineRenderer;
class Gizmos
{
public:
    enum GizmosPassType
    {
        Depth,
        Overlay
    };

    static void SetColor(const Color &color);

    static void SetPosition(const Vector3 &position);
    static void SetRotation(const Quaternion &rotation);
    static void SetScale(const Vector3 &scale);

    static void SetLineWidth(float lineWidth);
    static void SetDrawWireframe(bool wireframe);
    static void SetReceivesLighting(bool receivesLighting);
    static void SetBillboard();
    static void RenderCustomMesh(Mesh *m);
    static void RenderBox(const AABox &b);
    static void RenderSimpleBox(const AABox &b);
    static void RenderRect(const Rect &r);
    static void RenderFillRect(const Rect &r, const Color &fillColor);
    static void RenderIcon(const Texture2D *texture,
                           bool billboard = true);
    static void RenderScreenIcon(const Texture2D *texture,
                                 const Rect &screenRect,
                                 bool fixAspectRatio = false);
    static void RenderCircle(float radius);
    static void RenderScreenLine(const Vector2 &origin, const Vector2 &destiny);
    static void RenderLine(const Vector3 &origin, const Vector3 &destiny);
    static void RenderRay(const Vector3 &origin, const Vector3 &rayDir);
    static void RenderSphere(const Vector3 &origin, float radius);
    static void RenderFrustum(const Vector3 &forward, const Vector3 &up,
                              const Vector3 &origin,
                              float zNear, float zFar,
                              float fovDegrees, float aspectRatio);
    static void RenderSimpleSphere(const Vector3 &origin, float radius);

    static void Render(Renderer *rend);

private:
    Mesh *m_boxMesh      = nullptr;
    Mesh *m_sphereMesh   = nullptr;
    Mesh *m_planeMesh    = nullptr;
    Material *m_material = nullptr;

    GameObject *m_gizmosGameObject = nullptr;
    Transform  *m_transform = nullptr;
    SingleLineRenderer *m_singleLineRenderer = nullptr;
    CircleRenderer *m_circleRenderer = nullptr;
    MeshRenderer *m_meshRenderer = nullptr;
    List<Renderer*> m_renderers;

    bool m_resetAllowed  = true;

    Gizmos();

    void Init();

    void SetResetAllowed(bool allowed);
    void Reset();

    Gizmos *GetInstance();

    friend class Scene;
};

#endif // GIZMOS_H
