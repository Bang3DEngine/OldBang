#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Renderer.h"

class VAO;
class Mesh;
class Material;
class MeshRenderer : public Renderer
{
public:

    MeshRenderer();
    virtual ~MeshRenderer();

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    void SetMaterial(Material *m) override;
    virtual void SetMesh(Mesh *m);

    virtual AABox GetAABBox() const override;

    virtual const Mesh* GetMesh();
    virtual String GetName() const override;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

protected:
    Mesh *m_mesh = nullptr;

    virtual void RenderWithoutBindingMaterial() const override;

    friend class GameObject;
};

#endif // MESHRENDERER_H
