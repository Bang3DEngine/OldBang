#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Bang.h"

#include <glm/gtx/string_cast.hpp>

#include "VAO.h"
#include "Mesh.h"
#include "Component.h"
#include "Scene.h"
#include "Material.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "ShaderContract.h"
#include "AssetsManager.h"
#include "Renderer.h"

#ifdef BANG_EDITOR
#include "InspectorFileSW.h"
#endif


class MeshRenderer : public Renderer
{
private:
    Mesh *mesh = nullptr;

    void Render() const override;

protected:
    void _OnRender() override;

public:
    MeshRenderer();
    virtual ~MeshRenderer();

    void SetMaterial(Material *m) override;

    virtual void SetMesh(Mesh *m);
    virtual const Mesh* GetMesh();

    virtual const std::string ToString() const override { return "MeshRenderer"; }
    virtual std::string GetName() const override { return "MeshRenderer"; }

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* GetComponentInfo() override;
    virtual void OnSlotValueChanged(InspectorWidget *source) override;

    void Write(std::ostream &f) const;
    void Read(std::istream &f);
    #endif
};

#endif // MESHRENDERER_H
