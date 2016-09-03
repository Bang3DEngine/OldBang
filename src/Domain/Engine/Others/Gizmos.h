#ifndef GIZMOS_H
#define GIZMOS_H

#include "Bang.h"

#include "Box.h"
#include "Color.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

class Mesh;
class Material;
class Renderer;
class Texture2D;
class MeshRenderer;
class CircleRenderer;
class SingleLineRenderer;
class EditorGameObject;
class Gizmos
{
friend class Screen;
friend class EditorScene;
private:
    Gizmos() {}

    static std::vector<Renderer*> m_renderers;

    static void Init();

    static void SetGizmosGameObject(EditorGameObject *ego); // Called by scene

    static bool m_inited;

    static EditorGameObject *m_gizmosGameObject;
    static SingleLineRenderer *m_singleLineRenderer;
    static CircleRenderer *m_circleRenderer;
    static MeshRenderer *m_meshRenderer;
    static Mesh *m_boxMesh;
    static Mesh *m_sphereMesh;
    static Mesh *m_planeMesh;

    static Material *m_material;
    static Color m_color;
    static Vector3 m_position;
    static Quaternion m_rotation;
    static Vector3 m_scale;
    static float m_lineWidth;
    static bool m_wireframe;
    static bool m_ignoreModel, m_ignoreView, m_ignoreProjection;
    static bool m_receivesLighting;

    static void SetStatesBeforeRendering();

public:

    static void SetColor(const Color &color);

    static void SetPosition(const Vector3 &position);
    static void SetRotation(const Quaternion &rotation);
    static void SetScale(const Vector3 &scale);

    static void SetLineWidth(float lineWidth);
    static void SetDrawWireframe(bool wireframe);
    static void SetReceivesLighting(bool receivesLighting);
    static void SetIgnoreMatrices(bool ignoreModel, bool ignoreView,
                                  bool ignoreProjection);
    static void RenderCustomMesh(Mesh *m);
    static void RenderBox(const Box &b);
    static void RenderSimpleBox(const Box &b);
    static void RenderIcon(const Texture2D *texture,
                           bool billboard = true);
    static void RenderLine(const Vector3 &origin, const Vector3 &destiny);
    static void RenderRay(const Vector3 &origin, const Vector3 &rayDir);
    static void RenderSphere(const Vector3 &origin, float radius);
    static void RenderFrustum(const Vector3 &forward, const Vector3 &up,
                              const Vector3 &origin,
                              float zNear, float zFar,
                              float fovDegrees, float aspectRatio);
    static void RenderSimpleSphere(const Vector3 &origin, float radius);

    static void Begin();
};

#endif // GIZMOS_H
