#ifndef GIZMOS_H
#define GIZMOS_H

#include "AABox.h"
#include "Color.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

class Mesh;
class Rect;
class Material;
class Renderer;
class Texture2D;
class GameObject;
class MeshRenderer;
class CircleRenderer;
class SingleLineRenderer;
class EditorGizmosGameObject;
class Gizmos
{
public:
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

    static EditorGizmosGameObject *GetEditorGizmosGameObject();

    static void Render(Renderer *rend);

private:
    Gizmos() {}

    static void Init();

    /**
     * @brief Useful if we want to call for example inside RenderFrustum a lot
     * of RenderLines. We avoid Reset() inside RenderLines being called, because
     * we don't want this
     */
    static void SetResetAllowed(bool allowed);
    static void Reset();

    static bool IsGizmoRenderer(Renderer *rend);

    friend class Screen;
    friend class EditorScene;
    friend class GraphicPipeline;
    friend class SelectionFramebuffer;
};

#endif // GIZMOS_H
