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

public:

    static void SetColor(const Vector3 &color);
    static void DrawBox(const Box &b, bool wireframe = true);
};

#endif // GIZMOS_H
