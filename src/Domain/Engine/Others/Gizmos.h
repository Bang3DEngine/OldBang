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
class Texture2D;
class MeshRenderer;
class CircleRenderer;
class SingleLineRenderer;
class EditorGameObject;
class Gizmos
{
friend class Canvas;
friend class EditorScene;
private:
    Gizmos() {}

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
    static float m_lineWidth;
    static bool m_wireframe;
    static bool m_receivesLighting;

    static void SetStatesBeforeDrawing();

    static void OnNewFrame();

public:

    static void SetColor(const Color &color);
    static void SetLineWidth(float lineWidth);
    static void SetDrawWireframe(bool wireframe);
    static void SetReceivesLighting(bool receivesLighting);
    static void DrawBox(const Box &b, const Quaternion& rotation = Quaternion::identity);
    static void DrawSimpleBox(const Box &b, const Quaternion& rotation = Quaternion::identity);
    static void DrawIcon(const Texture2D *texture,
                         const Vector3 &position, const Vector3 &scale,
                         bool billboard = true);
    static void DrawLine(const Vector3 &origin, const Vector3 &destiny);
    static void DrawRay(const Vector3 &origin, const Vector3 &rayDir);
    static void DrawSphere(const Vector3 &origin, float radius);
    static void DrawFrustum(const Vector3 &forward, const Vector3 &up,
                            const Vector3 &origin,
                            float zNear, float zFar,
                            float fovDegrees, float aspectRatio);
    static void DrawSimpleSphere(const Vector3 &origin, float radius);
};

#endif // GIZMOS_H
