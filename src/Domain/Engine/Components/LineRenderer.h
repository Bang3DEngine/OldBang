#ifndef LINERENDERER_H
#define LINERENDERER_H

#include "Bang/AABox.h"
#include "Bang/Array.h"
#include "Bang/String.h"
#include "Bang/Vector3.h"
#include "Bang/Renderer.h"

class Mesh;
class XMLNode;
class Material;
class ICloneable;
class LineRenderer : public Renderer
{
    OBJECT(LineRenderer)
    COMPONENT_ICON(LineRenderer, "Icons/eye.png")

public:
    virtual ~LineRenderer();

    virtual AABox GetAABBox() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    Array<Vector3> m_points;

    LineRenderer();

    virtual void RefreshPoints();
    virtual void RenderWithoutMaterial() const override;

private:
    Mesh *m_linesMesh = nullptr;
};

#endif // LINERENDERER_H
