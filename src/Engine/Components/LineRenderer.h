#ifndef LINERENDERER_H
#define LINERENDERER_H

#include "Bang/Array.h"
#include "Bang/Vector3.h"
#include "Bang/MeshRenderer.h"

FORWARD class Mesh;
FORWARD class AABox;
FORWARD class XMLNode;
FORWARD class Material;
FORWARD class ICloneable;

class LineRenderer : public Renderer
{
    OBJECT(LineRenderer)

public:
    virtual ~LineRenderer();

    virtual void CloneInto(ICloneable *clone) const override;

    const Array<Vector3>& GetPoints() const;
    virtual void SetPoint(int i, const Vector3& point);
    virtual void SetPoints(const Array<Vector3>& points);
    virtual AABox GetAABBox() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    LineRenderer();

    virtual void Render() const override;

private:
    MeshRenderer  *m_meshRenderer = nullptr;
    Array<Vector3> m_points;
};

#endif // LINERENDERER_H
