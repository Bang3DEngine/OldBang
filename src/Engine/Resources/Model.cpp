#include "Bang/Model.h"

#include "Bang/Mesh.h"
#include "Bang/ModelIO.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/GameObject.h"
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

GameObject *Model::CreateGameObjectFromModel() const
{
    GameObject *gameObject = GameObjectFactory::CreateGameObject();

    for (int i = 0; i < SCAST<int>(GetMeshes().Size()); ++i)
    {
        GameObject *subGameObject = GameObjectFactory::CreateGameObject();
        MeshRenderer *mr = subGameObject->AddComponent<MeshRenderer>();
        mr->SetMesh(GetMeshes()[i].Get());
        mr->SetMaterial(GetMaterials()[i].Get());
        subGameObject->SetName( GetMeshesNames()[i] );
        subGameObject->SetParent(gameObject);
    }

    String name = GetResourceFilepath().GetName();
    if (!name.IsEmpty()) { gameObject->SetName(name); }

    return gameObject;
}

void Model::AddMesh(Mesh *mesh, Material *material,
                    const String &meshName, const String &materialName)
{
    String newMeshName = Model::GetNewName(meshName, GetMeshesNames());
    m_meshesNames.PushBack(newMeshName);
    m_meshes.PushBack( RH<Mesh>(mesh) );

    String newMaterialName = Model::GetNewName(materialName, GetMaterialsNames());
    m_materialsNames.PushBack(newMaterialName);
    m_materials.PushBack( RH<Material>(material) );
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
    return m_meshes;
}

const Array<RH<Material> > &Model::GetMaterials() const
{
    return m_materials;
}

const Array<String> &Model::GetMeshesNames() const
{
    return m_meshesNames;
}

const Array<String> &Model::GetMaterialsNames() const
{
    return m_materialsNames;
}

void Model::CloneInto(ICloneable *clone) const
{
    Asset::CloneInto(clone);

    Model *modelClone = DCAST<Model*>(clone);
    modelClone->m_meshes = m_meshes;
    modelClone->m_materials = m_materials;
}

void Model::Import(const Path &modelFilepath)
{
    Array< RH<Mesh> > meshes;
    Array< RH<Material> > materials;
    Array< String > meshesNames;
    Array< String > materialsNames;

    ModelIO::ReadModel(modelFilepath,
                       &meshes, &materials,
                       &meshesNames, &materialsNames);

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
