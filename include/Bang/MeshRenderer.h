#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Bang/Renderer.h"
#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

FORWARD class VAO;

class MeshRenderer : public Renderer
{
    COMPONENT(MeshRenderer)

public:
    void SetMesh(Mesh* m);
    Mesh* GetMesh() const;
    Mesh* GetSharedMesh() const;

    void SetCurrentLOD(uint lod);
    void SetAutoLOD(bool autoLOD);

    bool GetAutoLOD() const;
    uint GetCurrentLOD() const;

    // Renderer
    virtual AABox GetAABBox() const override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    mutable RH<Mesh> p_mesh;
    RH<Mesh> p_sharedMesh;

    bool m_autoLOD = false;
    uint m_currentLOD = 0;

    MeshRenderer();
    virtual ~MeshRenderer();

    // Renderer
    virtual void OnRender() override;

    Mesh* GetUserMesh() const;
};

NAMESPACE_BANG_END

#endif // MESHRENDERER_H
