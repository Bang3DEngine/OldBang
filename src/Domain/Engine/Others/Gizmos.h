#ifndef GIZMOS_H
#define GIZMOS_H

#include "Bang.h"

class Box;
class Mesh;
class Vector3;
class Material;
class Texture2D;
class MeshRenderer;
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
    static MeshRenderer *m_meshRenderer;
    static Mesh *m_boxMesh;
    static Mesh *m_planeMesh;

    static Material *m_material;
    static glm::vec4 m_color;
    static float m_lineWidth;
    static bool m_wireframe;
    static bool m_receivesLighting;

    static void SetStatesBeforeDrawing();

    static void OnNewFrame();

public:

    static void SetColor(const Vector3 &color);
    static void SetColor(const glm::vec4 &color);
    static void SetLineWidth(float lineWidth);
    static void SetDrawWireframe(bool wireframe);
    static void SetReceivesLighting(bool receivesLighting);
    static void DrawBox(const Box &b);
    static void DrawIcon(const Texture2D *texture,
                         const Vector3 &position, const Vector3 &scale,
                         bool billboard = true);
    static void DrawLine(const Vector3 &origin, const Vector3 &destiny);
};

#endif // GIZMOS_H
