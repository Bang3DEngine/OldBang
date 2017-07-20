#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include "Bang/Array.h"

class Mesh;
class Vector3;
class Material;
class GameObject;
class MeshRenderer;
class AssetsManager;
class GameObject;

class MeshFactory
{
public:

    static Mesh* GetPlane();
    static Mesh* GetUIPlane();
    static Mesh* GetCube();
    static Mesh* GetSphere();
    static Mesh* GetCone();

    static GameObject* GetPlaneGameObject();
    static GameObject* GetCubeGameObject();
    static GameObject* GetSphereGameObject();
    static GameObject* GetConeGameObject();

private:
    MeshFactory();
    static GameObject* CreatePrimitiveGameObject(Mesh *m, const String &name);
};

#endif // MESHFACTORY_H
