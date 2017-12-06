#include "Bang/MeshFactory.h"

#include "Bang/Mesh.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/GameObject.h"
#include "Bang/MeshRenderer.h"
#include "Bang/MaterialFactory.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

void MeshFactory::GetPlane(RH<Mesh> *mesh)
{ MeshFactory::GetMesh(mesh, "Meshes/Plane.obj"); }
void MeshFactory::GetUIPlane(RH<Mesh> *mesh)
{ MeshFactory::GetMesh(mesh, "Meshes/UIPlane.obj"); }
void MeshFactory::GetCube(RH<Mesh> *mesh)
{ MeshFactory::GetMesh(mesh, "Meshes/Cube.obj"); }
void MeshFactory::GetSphere(RH<Mesh> *mesh)
{ MeshFactory::GetMesh(mesh, "Meshes/Sphere.obj"); }
void MeshFactory::GetCone(RH<Mesh> *mesh)
{ MeshFactory::GetMesh(mesh, "Meshes/Cone.obj"); }

void MeshFactory::GetMesh(RH<Mesh> *mesh, const String &enginePath)
{
    RH<Mesh> originalMesh;
    Resources::Load<Mesh>(&originalMesh, EPATH(enginePath));
    Resources::Clone<Mesh>(originalMesh, mesh);
}

GameObject* MeshFactory::CreatePrimitiveGameObject(Mesh* m, const String &name)
{
    GameObject *go = GameObjectFactory::CreateGameObject(true);
    go->SetName(name);

    RH<Material> mat;
    MaterialFactory::GetDefault(&mat);
    MeshRenderer *r = go->AddComponent<MeshRenderer>();
    r->SetRenderPrimitive(GL::Primitives::Triangles);
    r->SetMaterial(mat.Get());
    r->SetMesh(m);

    return go;
}

GameObject* MeshFactory::GetPlaneGameObject()
{
    RH<Mesh> mesh;
    MeshFactory::GetPlane(&mesh);
    return CreatePrimitiveGameObject(mesh.Get(), "Plane");
}

GameObject* MeshFactory::GetCubeGameObject()
{
    RH<Mesh> mesh;
    MeshFactory::GetCube(&mesh);
    return CreatePrimitiveGameObject(mesh.Get(), "Cube");
}

GameObject* MeshFactory::GetSphereGameObject()
{
    RH<Mesh> mesh;
    MeshFactory::GetSphere(&mesh);
    return CreatePrimitiveGameObject(mesh.Get(), "Sphere");
}

GameObject* MeshFactory::GetConeGameObject()
{
    RH<Mesh> mesh;
    MeshFactory::GetCone(&mesh);
    return CreatePrimitiveGameObject(mesh.Get(), "Cone");
}
