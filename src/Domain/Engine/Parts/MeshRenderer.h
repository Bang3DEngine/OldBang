#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Bang.h"

#include <glm/gtx/string_cast.hpp>

#include "VAO.h"
#include "Mesh.h"
#include "Part.h"
#include "Stage.h"
#include "Material.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "ShaderContract.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "InspectorPartAssetSW.h"
#endif


class MeshRenderer : public Part
{
private:

    Mesh *mesh = nullptr;
    Material *material = nullptr;

protected:

    void _OnRender() override;

public:
    MeshRenderer();
    virtual ~MeshRenderer();

    void Render(Mesh::RenderMode drawingMode = Mesh::RenderMode::Triangles) const;

    void SetMesh(Mesh *m);
    const Mesh* GetMesh();

    void SetMaterial(Material *m);
    Material* GetMaterial();

    void Write(std::ostream &f) const;
    void Read(std::istream &f);

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override { return "MeshRenderer"; }

    #ifdef BANG_EDITOR
    virtual InspectorPartInfo* GetInfo() override;
    virtual void OnInspectorSlotChanged(InspectorPartWidget *partWidget) override;
    #endif
};

#endif // MESHRENDERER_H
