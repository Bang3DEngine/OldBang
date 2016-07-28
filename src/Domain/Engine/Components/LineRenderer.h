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
protected:
    std::vector<Vector3> m_points;

    VBO *m_vbo = nullptr;
    VAO *m_vao = nullptr;

    RenderMode m_drawLinesMode = RenderMode::Lines;

    virtual void BindPointsToVAO() const;
    virtual void RenderWithoutBindingMaterial() const override;

public:
    LineRenderer();
    virtual ~LineRenderer();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override;
    virtual ICloneable *Clone() const override = 0;

    virtual void SetMaterial(Material *m) override;

    virtual Box GetBoundingBox() const override;
    const std::vector<Vector3>& GetPoints() const;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // LINERENDERER_H
