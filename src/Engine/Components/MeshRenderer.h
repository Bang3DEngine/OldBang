#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Bang/Renderer.h"

FORWARD class G_VAO;

class MeshRenderer : public Renderer
{
    COMPONENT(MeshRenderer)

public:
    virtual ~MeshRenderer();

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void SetMesh(Mesh *m);

    virtual AABox GetAABBox() const override;

    virtual Mesh* GetMesh() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    mutable Mesh *p_mesh = nullptr;

    MeshRenderer();
    virtual void OnRender() override;
};

#endif // MESHRENDERER_H
