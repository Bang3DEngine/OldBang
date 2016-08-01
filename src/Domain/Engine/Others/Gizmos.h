#ifndef GIZMOS_H
#define GIZMOS_H

#include "Bang.h"

class Box;
class Mesh;
class Vector3;
class Material;
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

    static Material *m_material;
    static Vector3 m_color;
    static float m_lineWidth;
    static bool m_receivesLighting;

    static void SetStatesBeforeDrawing();

    static void OnNewFrame();

public:

    static void SetColor(const Vector3 &color);
    static void SetLineWidth(float lineWidth);
    static void SetReceivesLighting(bool receivesLighting);
    static void DrawBox(const Box &b, bool wireframe = true);
    static void DrawLine(const Vector3 &origin, const Vector3 &destiny);
};

#endif // GIZMOS_H
