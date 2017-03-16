#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Renderer.h"

class VAO;
class Mesh;
class Material;
class MeshRenderer : public Renderer
{
    ICLONEABLE(MeshRenderer)

public:
    MeshRenderer();
    virtual ~MeshRenderer();

    virtual void CloneInto(ICloneable *clone) const override;

    void SetMaterial(Material *m) override;
    virtual void SetMesh(Mesh *m);

    virtual AABox GetAABBox() const override;

    virtual const Mesh* GetMesh();
    virtual String GetName() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    mutable Mesh *m_mesh = nullptr;

    void BindCurrentMeshToShaderProgram() const;
    virtual void RenderWithoutMaterial() const override;

    friend class GameObject;
};

#endif // MESHRENDERER_H
