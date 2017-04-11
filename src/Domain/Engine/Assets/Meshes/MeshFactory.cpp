#include "Bang/MeshFactory.h"

#include "Bang/Mesh.h"
#include "Bang/Vector3.h"
#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/MeshRenderer.h"
#include "Bang/AssetsManager.h"

#ifdef BANG_EDITOR
#include "Bang/GameObject.h"
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

    Material *mat = AssetsManager::Load<Material>("Materials/G_Default.bmat",
                                                  true);

    MeshRenderer *r = go->AddComponent<MeshRenderer>();
    r->SetRenderMode(GL::RenderMode::Triangles);
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
