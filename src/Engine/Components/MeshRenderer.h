#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Bang/Renderer.h"

FORWARD class Mesh;
FORWARD class Material;
FORWARD class G_VAO;

class MeshRenderer : public Renderer
{
    OBJECT(MeshRenderer)

public:
    MeshRenderer();
    virtual ~MeshRenderer();

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void SetMesh(Mesh *m);

    virtual AABox GetAABBox() const override;

    virtual Mesh* GetMesh() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    mutable Mesh *p_mesh = nullptr;

    virtual void Render() const override;
};

#endif // MESHRENDERER_H
