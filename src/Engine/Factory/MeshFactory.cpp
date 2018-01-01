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
    return mf->m_cache.Get(enginePath);
}

MeshFactory *MeshFactory::GetActive()
{
    return Resources::GetActive()->m_meshFactory;
}
