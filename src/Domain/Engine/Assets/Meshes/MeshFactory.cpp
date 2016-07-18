#include "MeshFactory.h"

#include "Mesh.h"
#include "Vector3.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "EditorGameObject.h"
#endif

Mesh* MeshFactory::GetPlane()
{
    return AssetsManager::GetAsset<Mesh>("Assets/Engine/Meshes/Plane.bmesh");
}

Mesh* MeshFactory::GetCube()
{
    return AssetsManager::GetAsset<Mesh>("Assets/Engine/Meshes/Cube.bmesh");
}

Mesh *MeshFactory::GetSphere()
{
    return AssetsManager::GetAsset<Mesh>("Assets/Engine/Meshes/Sphere.bmesh");
}

Mesh *MeshFactory::GetCone()
{
    return AssetsManager::GetAsset<Mesh>("Assets/Engine/Meshes/Cone.bmesh");
}

GameObject* MeshFactory::CreatePrimitiveGameObject(Mesh *m, const std::string &name)
{
    GameObject *go = new GameObject(name);

    Material *mat = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/default.bmat");

    MeshRenderer *r = go->AddComponent<MeshRenderer>();
    r->SetRenderMode(MeshRenderer::RenderMode::Triangles);
    r->SetMaterial(mat);
    r->SetMesh(m);

    return go;
}


#ifdef BANG_EDITOR
EditorGameObject* MeshFactory::CreatePrimitiveEditorGameObject(Mesh *m, const std::string &name)
{
    EditorGameObject *go = new EditorGameObject(name);

    Material *mat = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/default.bmat");

    MeshRenderer *r = go->AddComponent<MeshRenderer>();
    r->SetRenderMode(MeshRenderer::RenderMode::Triangles);
    r->SetMaterial(mat);
    r->SetMesh(m);

    return go;
}
#endif

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


#ifdef BANG_EDITOR
EditorGameObject *MeshFactory::GetPlaneEditorGameObject()
{
    return CreatePrimitiveEditorGameObject(GetPlane(), "Plane");
}

EditorGameObject *MeshFactory::GetCubeEditorGameObject()
{
    return CreatePrimitiveEditorGameObject(GetCube(), "Cube");
}

EditorGameObject *MeshFactory::GetSphereEditorGameObject()
{
    return CreatePrimitiveEditorGameObject(GetSphere(), "Sphere");
}

EditorGameObject *MeshFactory::GetConeEditorGameObject()
{
    return CreatePrimitiveEditorGameObject(GetCone(), "Cone");
}
#endif
