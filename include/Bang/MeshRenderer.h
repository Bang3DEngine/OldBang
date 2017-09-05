#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Bang/Renderer.h"

FORWARD class G_VAO;

class MeshRenderer : public Renderer
{
    COMPONENT(MeshRenderer)

public:
    virtual ~MeshRenderer();

    void SetMesh(Mesh *m);
    Mesh* GetMesh() const;

    // Renderer
    virtual AABox GetAABBox() const override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    mutable Mesh *p_mesh = nullptr;

    MeshRenderer();

    // Renderer
    virtual void OnRender() override;
};

#endif // MESHRENDERER_H