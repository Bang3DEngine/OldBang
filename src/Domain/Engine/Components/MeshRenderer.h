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

protected:

    virtual void ActivateStatesBeforeRendering() const override;
    virtual void RenderWithoutBindingMaterial() const override;
    virtual void OnRender() override;
    virtual void Render() const override;

public:
    MeshRenderer();
    virtual ~MeshRenderer();

    void SetMaterial(Material *m) override;
    virtual void SetMesh(Mesh *m);

    virtual const Mesh* GetMesh();
    virtual const std::string ToString() const override;
    virtual std::string GetName() const override;

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* GetComponentInfo() override;
    virtual void OnSlotValueChanged(InspectorWidget *source) override;

    void Write(std::ostream &f) const;
    void Read(std::istream &f);
    #endif
};

#endif // MESHRENDERER_H
