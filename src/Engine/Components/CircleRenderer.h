#ifndef CIRCLERENDERER_H
#define CIRCLERENDERER_H

#include "Bang/LineRenderer.h"

class CircleRenderer : public LineRenderer
{
    COMPONENT(CircleRenderer)

public:
    virtual ~CircleRenderer();

    void SetRadius(float m_radius);
    void SetSegments(int m_segments);

    float GetRadius() const;
    int GetSegments() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    float m_radius = 1.0f;
    int m_segments = 32;

    CircleRenderer();

    virtual void ReloadPoints();
};

#endif // CIRCLERENDERER_H
