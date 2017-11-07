#ifndef CIRCLERENDERER_H
#define CIRCLERENDERER_H

#include "Bang/LineRenderer.h"

NAMESPACE_BANG_BEGIN

class CircleRenderer : public LineRenderer
{
    COMPONENT(CircleRenderer)

public:

    void SetRadius(float m_radius);
    void SetSegments(int m_segments);

    float GetRadius() const;
    int GetSegments() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    float m_radius = 1.0f;
    int m_segments = 32;

    CircleRenderer();
    virtual ~CircleRenderer();

    virtual void ReloadPoints();
};

NAMESPACE_BANG_END

#endif // CIRCLERENDERER_H
