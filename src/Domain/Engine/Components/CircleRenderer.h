#ifndef CIRCLERENDERER_H
#define CIRCLERENDERER_H

#include <vector>

#include "VBO.h"
#include "VAO.h"
#include "Mesh.h"
#include "Component.h"
#include "Scene.h"
#include "Material.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "ShaderContract.h"
#include "AssetsManager.h"
#include "FileReader.h"
#include "ShaderContract.h"

#include "Renderer.h"

class CircleRenderer : public Renderer
{
private:

    float radius = 1.0f;
    int segments = 32;

    VBO *vbo = nullptr;
    VAO *vao = nullptr;

    void GenerateVertices(std::vector<Vector3> *vertices) const;
    void BindPointsToVAO() const;

protected:

    virtual void ActivateStatesBeforeRendering() const override;
    virtual void RenderWithoutBindingMaterial() const override;
    virtual void OnRender() override;
    virtual void Render() const override;

public:
    CircleRenderer();
    virtual ~CircleRenderer();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override;

    virtual void SetMaterial(Material *m) override;

    virtual Box GetBoundingBox() const override;

    void SetRadius(float radius);
    void SetSegments(int segments);

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* GetComponentInfo() override;
    virtual void OnSlotValueChanged(InspectorWidget *source) override;

    void Write(std::ostream &f) const;
    void Read(std::istream &f);
    #endif
};

#endif // CIRCLERENDERER_H
