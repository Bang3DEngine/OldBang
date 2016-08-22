#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include <vector>

#include "Bang.h"

class Mesh;
class Vector3;
class Material;
class GameObject;
class MeshRenderer;
class AssetsManager;
class EditorGameObject;

class MeshFactory
{
friend class MenuBar;

private:
    MeshFactory() {}
    static GameObject* CreatePrimitiveGameObject(Mesh *m, const String &name);

    #ifdef BANG_EDITOR
    static EditorGameObject* CreatePrimitiveEditorGameObject(Mesh *m, const String &name);
    #endif

public:

    static Mesh* GetPlane();
    static Mesh* GetCube();
    static Mesh* GetSphere();
    static Mesh* GetCone();

    static GameObject* GetPlaneGameObject();
    static GameObject* GetCubeGameObject();
    static GameObject* GetSphereGameObject();
    static GameObject* GetConeGameObject();

    #ifdef BANG_EDITOR
    static EditorGameObject* GetPlaneEditorGameObject();
    static EditorGameObject* GetCubeEditorGameObject();
    static EditorGameObject* GetSphereEditorGameObject();
    static EditorGameObject* GetConeEditorGameObject();
    #endif
};

#endif // MESHFACTORY_H
