#include "Bang/MeshFactory.h"

#include "Bang/Mesh.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/GameObject.h"
#include "Bang/MeshRenderer.h"
#include "Bang/MaterialFactory.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

RH<Mesh> MeshFactory::GetPlane()
{ return MeshFactory::GetMesh("Meshes/Plane.obj"); }
RH<Mesh> MeshFactory::GetUIPlane()
{ return MeshFactory::GetMesh("Meshes/UIPlane.obj"); }
RH<Mesh> MeshFactory::GetCube()
{ return MeshFactory::GetMesh("Meshes/Cube.obj"); }
RH<Mesh> MeshFactory::GetSphere()
{ return MeshFactory::GetMesh("Meshes/Sphere.obj"); }
RH<Mesh> MeshFactory::GetCone()
{ return MeshFactory::GetMesh("Meshes/Cone.obj"); }

RH<Mesh> MeshFactory::GetMesh(const String &enginePath)
{
    MeshFactory *mf = MeshFactory::GetActive();
    mf->m_cache.Add(enginePath, Resources::Load<Mesh>(EPATH(enginePath)));
    return Resources::Clone<Mesh>( mf->m_cache.Get(enginePath) );
}

GameObject* MeshFactory::CreatePrimitiveGameObject(Mesh* m, const String &name)
{
    GameObject *go = GameObjectFactory::CreateGameObject(true);
    go->SetName(name);

    MeshRenderer *r = go->AddComponent<MeshRenderer>();
    r->SetRenderPrimitive(GL::Primitive::Triangles);
    r->SetMaterial(MaterialFactory::GetDefault().Get());
    r->SetMesh(m);

    return go;
}

MeshFactory *MeshFactory::GetActive()
{
    return Resources::GetActive()->m_meshFactory;
}

GameObject* MeshFactory::GetPlaneGameObject()
{
    RH<Mesh> mesh = MeshFactory::GetPlane();
    return CreatePrimitiveGameObject(mesh.Get(), "Plane");
}

GameObject* MeshFactory::GetCubeGameObject()
{
    RH<Mesh> mesh = MeshFactory::GetCube();
    return CreatePrimitiveGameObject(mesh.Get(), "Cube");
}

GameObject* MeshFactory::GetSphereGameObject()
{
    RH<Mesh> mesh = MeshFactory::GetSphere();
    return CreatePrimitiveGameObject(mesh.Get(), "Sphere");
}

GameObject* MeshFactory::GetConeGameObject()
{
    RH<Mesh> mesh = MeshFactory::GetCone();
    return CreatePrimitiveGameObject(mesh.Get(), "Cone");
}
