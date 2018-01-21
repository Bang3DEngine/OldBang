#ifndef MODEL_H
#define MODEL_H

#include "Bang/Asset.h"
#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

class Model : public Asset
{
    ASSET(Model)

public:
	Model();
	virtual ~Model();

    void AddMesh(Mesh *mesh, Material *material);

    const Array< RH<Mesh> >& GetMeshes() const;
    const Array< RH<Material> >& GetMaterials() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Resource
    void Import(const Path &modelFilepath) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    Array< RH<Mesh> > m_meshes;
    Array< RH<Material> > m_materials;
};

NAMESPACE_BANG_END

#endif // MODEL_H

