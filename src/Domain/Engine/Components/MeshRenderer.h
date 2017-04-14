#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Bang/Renderer.h"

class VAO;
class Mesh;
class Material;
class MeshRenderer : public Renderer
{
    OBJECT(MeshRenderer)
    COMPONENT_ICON(MeshRenderer, "Icons/eye.png")

public:
    MeshRenderer();
    virtual ~MeshRenderer();

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void SetMesh(Mesh *m);

    virtual AABox GetAABBox() const override;

    virtual const Mesh* GetMesh();

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    mutable Mesh *m_mesh = nullptr;

    virtual void RenderWithoutMaterial() const override;
};

#endif // MESHRENDERER_H
