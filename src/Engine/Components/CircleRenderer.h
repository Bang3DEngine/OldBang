#ifndef CIRCLERENDERER_H
#define CIRCLERENDERER_H

#include "Bang/Math.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/LineRenderer.h"

class XMLNode;
class ICloneable;
class CircleRenderer : public LineRenderer
{
    OBJECT(CircleRenderer)

public:
    virtual ~CircleRenderer();

    virtual void CloneInto(ICloneable *clone) const override;

    void SetRadius(float m_radius);
    void SetSegments(int m_segments);

    float GetRadius() const;
    int GetSegments() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    CircleRenderer();

    float m_radius = 1.0f;
    int m_segments = 32;

    virtual void ReloadPoints();
};

#endif // CIRCLERENDERER_H
