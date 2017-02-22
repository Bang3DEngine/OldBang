#include "MeshFactory.h"

#include "Mesh.h"
#include "Vector3.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "GameObject.h"
#endif

Mesh* MeshFactory::GetPlane()
{
    return AssetsManager::Load<Mesh>("Meshes/Plane.bmesh", true);
}

Mesh *MeshFactory::GetUIPlane()
{
    return AssetsManager::Load<Mesh>("Meshes/UIPlane.bmesh", true);
}

Mesh* MeshFactory::GetCube()
{
    return AssetsManager::Load<Mesh>("Meshes/Cube.bmesh", true);
}

Mesh *MeshFactory::GetSphere()
{
    return AssetsManager::Load<Mesh>("Meshes/Sphere.bmesh", true);
}

Mesh *MeshFactory::GetCone()
{
    return AssetsManager::Load<Mesh>("Meshes/Cone.bmesh", true);
}

MeshFactory::MeshFactory() {}

GameObject* MeshFactory::CreatePrimitiveGameObject(Mesh *m, const String &name)
{
    GameObject *go = new GameObject(name);

    Material *mat = AssetsManager::Load<Material>("Materials/G_Default.bmat", true);

    MeshRenderer *r = go->AddComponent<MeshRenderer>();
    r->SetRenderMode(MeshRenderer::RenderMode::Triangles);
    r->SetMaterial(mat);
    r->SetMesh(m);

    return go;
}

GameObject *MeshFactory::GetPlaneGameObject()
{
    return CreatePrimitiveGameObject(GetPlane(), "Plane");
}

GameObject *MeshFactory::GetCubeGameObject()
{
    return CreatePrimitiveGameObject(GetCube(), "Cube");
}

GameObject *MeshFactory::GetSphereGameObject()
{
    return CreatePrimitiveGameObject(GetSphere(), "Sphere");
}

GameObject *MeshFactory::GetConeGameObject()
{
    return CreatePrimitiveGameObject(GetCone(), "Cone");
}
