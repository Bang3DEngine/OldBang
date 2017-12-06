#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

class MeshFactory
{
public:
    static void GetPlane(RH<Mesh> *mesh);
    static void GetUIPlane(RH<Mesh> *mesh);
    static void GetCube(RH<Mesh> *mesh);
    static void GetSphere(RH<Mesh> *mesh);
    static void GetCone(RH<Mesh> *mesh);

    static GameObject* GetPlaneGameObject();
    static GameObject* GetCubeGameObject();
    static GameObject* GetSphereGameObject();
    static GameObject* GetConeGameObject();

private:
    MeshFactory() = default;
    static void GetMesh(RH<Mesh> *mesh, const String &enginePath);

    static GameObject* CreatePrimitiveGameObject(Mesh* m, const String &name);
};

NAMESPACE_BANG_END

#endif // MESHFACTORY_H
