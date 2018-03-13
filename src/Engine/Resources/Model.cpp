#include "Bang/Model.h"

#include "Bang/Mesh.h"
#include "Bang/ModelIO.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/GUIDManager.h"
#include "Bang/MeshRenderer.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

// Model
Model::Model()
{
}

Model::~Model()
{
}

GameObject *CreateGameObjectFromModelNodeTree(const ModelIOScene &modelScene,
                                              const Tree<ModelIONode>* modelTree)
{
    const ModelIONode &modelNode = modelTree->GetData();
    GameObject *gameObject = GameObjectFactory::CreateGameObject();

    // Set name
    gameObject->SetName( modelNode.name );

    // Set transform
    gameObject->GetTransform()->SetLocalPosition(
                Transform::GetPositionFromMatrix4(modelNode.transformation) );
    gameObject->GetTransform()->SetLocalRotation(
                Transform::GetRotationFromMatrix4(modelNode.transformation) );
    gameObject->GetTransform()->SetLocalScale(
                Transform::GetScaleFromMatrix4(modelNode.transformation) );

    // Add mesh renderers
    for (int i = 0; i < SCAST<int>(modelNode.meshIndices.Size()); ++i)
    {
        MeshRenderer *mr = gameObject->AddComponent<MeshRenderer>();
        mr->SetMesh( modelScene.meshes[ modelNode.meshIndices[i] ].Get() );
        mr->SetMaterial( modelScene.materials[ modelNode.meshIndices[i] ].Get() );
    }

    // Add children
    for (const Tree<ModelIONode> *childTree : modelTree->GetChildren())
    {
        GameObject *childGo = CreateGameObjectFromModelNodeTree(modelScene,
                                                                childTree);
        childGo->SetParent(gameObject);
    }

    return gameObject;
}

GameObject *Model::CreateGameObjectFromModel() const
{
    return CreateGameObjectFromModelNodeTree(m_modelScene, m_modelScene.modelTree);
}

void Model::AddMesh(Mesh *mesh, Material *material,
                    const String &meshName, const String &materialName)
{
    String newMeshName = Model::GetNewName(meshName, GetMeshesNames());
    m_modelScene.meshesNames.PushBack(newMeshName);
    m_modelScene.meshes.PushBack( RH<Mesh>(mesh) );

    String newMaterialName = Model::GetNewName(materialName, GetMaterialsNames());
    m_modelScene.materialsNames.PushBack(newMaterialName);
    m_modelScene.materials.PushBack( RH<Material>(material) );
}

RH<Mesh> Model::GetMeshByName(const String &meshName)
{
    RH<Mesh> mesh;
    for (uint i = 0; i < GetMeshesNames().Size(); ++i)
    {
        if (meshName == GetMeshesNames()[i])
        {
            mesh.Set(GetMeshes()[i].Get());
            break;
        }
    }
    return mesh;
}

RH<Material> Model::GetMaterialByName(const String &materialName)
{
    RH<Material> material;
    for (uint i = 0; i < GetMaterialsNames().Size(); ++i)
    {
        if (materialName == GetMaterialsNames()[i])
        {
            material.Set(GetMaterials()[i].Get());
            break;
        }
    }
    return material;
}


const Array<RH<Mesh> > &Model::GetMeshes() const
{
    return m_modelScene.meshes;
}

const Array<RH<Material> > &Model::GetMaterials() const
{
    return m_modelScene.materials;
}

const Array<String> &Model::GetMeshesNames() const
{
    return m_modelScene.meshesNames;
}

const Array<String> &Model::GetMaterialsNames() const
{
    return m_modelScene.materialsNames;
}

GUID::GUIDType Model::GetNextInsideFileGUID() const
{
    return m_modelScene.meshes.Size() + m_modelScene.materials.Size();
}

Resource *Model::GetInsideFileResource(GUID::GUIDType insideFileGUID) const
{
    auto pair = GetInsideFileResourceAndName(insideFileGUID);
    return pair.first;
}

String Model::GetInsideFileResourceName(GUID::GUIDType insideFileGUID) const
{
    auto pair = GetInsideFileResourceAndName(insideFileGUID);
    return pair.second;
}

void Model::Import(const Path &modelFilepath)
{
    Array< RH<Mesh> > meshes;
    Array< RH<Material> > materials;
    Array< String > meshesNames;
    Array< String > materialsNames;

    ModelIO::ReadModel(modelFilepath, GetGUID(), &m_modelScene);

    for (uint i = 0; i < meshes.Size(); ++i)
    {
        AddMesh(meshes[i].Get(), materials[i].Get(),
                meshesNames[i], materialsNames[i]);
    }
}

void Model::ImportXML(const XMLNode &xmlInfo)
{
    Asset::ImportXML(xmlInfo);
}

void Model::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);
}

std::pair<Resource *, String>
Model::GetInsideFileResourceAndName(GUID::GUIDType insideFileGUID) const
{
    std::pair<Resource*, String> pair;
    pair.first = nullptr;
    pair.second = "NameNotFound";

    ASSERT(GetMeshes().Size() == GetMaterials().Size());
    ASSERT(GetMeshes().Size() == GetMeshesNames().Size());
    ASSERT(GetMeshes().Size() == GetMaterialsNames().Size());
    Array< String > names;
    Array< Resource* > resources;
    for (uint i = 0; i < GetMeshes().Size(); ++i)
    {
        names.PushBack(GetMeshesNames()[i]);
        names.PushBack(GetMaterialsNames()[i]);
        resources.PushBack(GetMeshes()[i].Get());
        resources.PushBack(GetMaterials()[i].Get());
    }

    for (uint i = 0; i < resources.Size(); ++i)
    {
        if (resources[i]->GetGUID().GetInsideFileGUID() == insideFileGUID)
        {
            pair.first = resources[i];
            pair.second = names[i];
            break;
        }
    }

    return pair;
}

String Model::GetNewName(const String &originalName,
                         const Array<String> &existingNames)
{
    int auxIndex = -1;
    String newName = originalName;
    while (existingNames.Contains(newName))
    {
        ++auxIndex;
        newName = originalName + "_" + String(auxIndex);
    }
    return newName;
}
