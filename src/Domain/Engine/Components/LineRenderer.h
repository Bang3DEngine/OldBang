#ifndef LINERENDERER_H
#define LINERENDERER_H

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

class LineRenderer : public Renderer
{
private:
    std::vector<glm::vec3> points;

    VBO *vbo = nullptr;
    VAO *vao = nullptr;

    float width;

    void BindPointsToVAO() const;

protected:

    virtual void _OnRender() override;

public:
    LineRenderer();
    virtual ~LineRenderer();

    virtual void Render() const override;

    virtual const std::string ToString() const override { return "LineRenderer"; }
    virtual std::string GetName() const override { return "LineRenderer"; }

    virtual void SetMaterial(Material *m) override;

    void SetOrigin(glm::vec3 o);
    void SetDestiny(glm::vec3 d);

    float GetLineWidth() const;
    void SetLineWidth(float w);

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* GetComponentInfo() override;
    virtual void OnSlotValueChanged(InspectorWidget *source) override;

    void Write(std::ostream &f) const;
    void Read(std::istream &f);
    #endif
};

#endif // LINERENDERER_H
