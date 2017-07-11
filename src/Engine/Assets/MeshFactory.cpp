#include "Bang/MeshFactory.h"

#include "Bang/Mesh.h"
#include "Bang/Vector3.h"
#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/MeshRenderer.h"
#include "Bang/AssetsManager.h"

Mesh* MeshFactory::GetPlane()
{
    return AssetsManager::Load<Mesh>( EPATH("Meshes/Plane.bmesh") );
}

Mesh *MeshFactory::GetUIPlane()
{
    return AssetsManager::Load<Mesh>( EPATH("Meshes/UIPlane.bmesh") );
}

Mesh* MeshFactory::GetCube()
{
    return AssetsManager::Load<Mesh>( EPATH("Meshes/Cube.bmesh") );
}

Mesh *MeshFactory::GetSphere()
{
    return AssetsManager::Load<Mesh>( EPATH("Meshes/Sphere.bmesh") );
}

Mesh *MeshFactory::GetCone()
{
    return AssetsManager::Load<Mesh>( EPATH("Meshes/Cone.bmesh") );
}

MeshFactory::MeshFactory() {}

GameObject* MeshFactory::CreatePrimitiveGameObject(Mesh *m, const String &name)
{
    GameObject *go = new GameObject(name);

    Material *mat =
            AssetsManager::Load<Material>( EPATH("Materials/G_Default.bmat") );

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
