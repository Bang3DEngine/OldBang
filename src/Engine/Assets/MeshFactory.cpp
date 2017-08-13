#include "Bang/MeshFactory.h"

#include "Bang/Mesh.h"
#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/MeshRenderer.h"
#include "Bang/Resources.h"
#include "Bang/MaterialFactory.h"

Mesh* MeshFactory::GetPlane()
{
    return Resources::Load<Mesh>( EPATH("Meshes/Plane.bmesh") );
}

Mesh *MeshFactory::GetUIPlane()
{
    return Resources::Load<Mesh>( EPATH("Meshes/UIPlane.bmesh") );
}

Mesh* MeshFactory::GetCube()
{
    return Resources::Load<Mesh>( EPATH("Meshes/Cube.bmesh") );
}

Mesh *MeshFactory::GetSphere()
{
    return Resources::Load<Mesh>( EPATH("Meshes/Sphere.bmesh") );
}

Mesh *MeshFactory::GetCone()
{
    return Resources::Load<Mesh>( EPATH("Meshes/Cone.bmesh") );
}

MeshFactory::MeshFactory() {}

GameObject* MeshFactory::CreatePrimitiveGameObject(Mesh *m, const String &name)
{
    GameObject *go = new GameObject(name);

    Material *mat = MaterialFactory::GetDefault();
    MeshRenderer *r = go->AddComponent<MeshRenderer>();
    r->SetRenderPrimitive(GL::RenderPrimitive::Triangles);
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
