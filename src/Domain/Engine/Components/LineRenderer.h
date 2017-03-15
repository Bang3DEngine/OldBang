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

    virtual String GetName() const override;

    virtual ICloneable *Clone() const override;

    virtual void SetMaterial(Material *m) override;

    virtual AABox GetAABBox() const override;
    const Array<Vector3>& GetPoints() const;

    virtual void Read(const XMLNode *xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    Array<Vector3> m_points;

    VBO *m_vbo = nullptr;
    VAO *m_vao = nullptr;

    GL::RenderMode m_drawLinesMode = GL::RenderMode::Lines;

    virtual void BindPointsToVAO() const;
    virtual void RenderWithoutMaterial() const override;

    LineRenderer();

    friend class GameObject;
};

#endif // LINERENDERER_H
