#ifndef LINERENDERER_H
#define LINERENDERER_H

#include "AABox.h"
#include "Array.h"
#include "String.h"
#include "Vector3.h"
#include "Renderer.h"

class VBO;
class VAO;
class XMLNode;
class Material;
class ICloneable;
class LineRenderer : public Renderer
{
public:
    virtual ~LineRenderer();

    virtual const String ToString() const override;
    virtual String GetName() const override;

    virtual ICloneable *Clone() const override = 0;

    virtual void SetMaterial(Material *m) override;

    virtual AABox GetAABBox() const override;
    const Array<Vector3>& GetPoints() const;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

protected:
    Array<Vector3> m_points;

    VBO *m_vbo = nullptr;
    VAO *m_vao = nullptr;

    RenderMode m_drawLinesMode = RenderMode::Lines;

    virtual void BindPointsToVAO() const;
    virtual void RenderWithoutBindingMaterial() const override;

    LineRenderer();

    friend class GameObject;
};

#endif // LINERENDERER_H
