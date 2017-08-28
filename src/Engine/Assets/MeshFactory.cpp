#include "Bang/MeshFactory.h"

#include "Bang/Mesh.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/GameObject.h"
#include "Bang/MeshRenderer.h"
#include "Bang/MaterialFactory.h"
#include "Bang/GameObjectFactory.h"

Mesh* MeshFactory::GetPlane()
{
    return Resources::Load<Mesh>( EPATH("Meshes/Plane.obj") );
}

Mesh *MeshFactory::GetUIPlane()
{
    return Resources::Load<Mesh>( EPATH("Meshes/UIPlane.obj") );
}

Mesh* MeshFactory::GetCube()
{
    return Resources::Load<Mesh>( EPATH("Meshes/Cube.obj") );
}

Mesh *MeshFactory::GetSphere()
{
    return Resources::Load<Mesh>( EPATH("Meshes/Sphere.obj") );
}

Mesh *MeshFactory::GetCone()
{
    return Resources::Load<Mesh>( EPATH("Meshes/Cone.obj") );
}

MeshFactory::MeshFactory() {}

GameObject* MeshFactory::CreatePrimitiveGameObject(Mesh *m, const String &name)
{
    GameObject *go = GameObjectFactory::CreateGameObject(true);
    go->SetName(name);

    Material *mat = MaterialFactory::GetDefault();
    MeshRenderer *r = go->AddComponent<MeshRenderer>();
    r->SetRenderPrimitive(GL::Primitives::Triangles);
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
