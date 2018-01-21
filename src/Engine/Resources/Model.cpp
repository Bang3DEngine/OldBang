#include "Bang/Model.h"

#include "Bang/Mesh.h"
#include "Bang/ModelIO.h"
#include "Bang/Material.h"

USING_NAMESPACE_BANG

// Model
Model::Model()
{
}

Model::~Model()
{
}

void Model::AddMesh(Mesh *mesh, Material *material)
{
    m_meshes.PushBack( RH<Mesh>(mesh) );
    m_materials.PushBack( RH<Material>(material) );
}

const Array<RH<Mesh> > &Model::GetMeshes() const
{
    return m_meshes;
}

const Array<RH<Material> > &Model::GetMaterials() const
{
    return m_materials;
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
    ModelIO::ReadModel(modelFilepath, &m_meshes, &m_materials);
}

void Model::ImportXML(const XMLNode &xmlInfo)
{
    Asset::ImportXML(xmlInfo);
}

void Model::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);
}
