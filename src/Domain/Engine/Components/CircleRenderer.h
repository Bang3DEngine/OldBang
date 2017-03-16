#ifndef CIRCLERENDERER_H
#define CIRCLERENDERER_H

#include "Math.h"
#include "String.h"
#include "Vector2.h"
#include "LineRenderer.h"

class XMLNode;
class ICloneable;
class CircleRenderer : public LineRenderer
{
public:
    virtual ~CircleRenderer();

    virtual String GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    ICloneable *CloneVirtual() const override;

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

    virtual void GeneratePoints();

    friend class GameObject;
    friend class ICloneable;
    friend class EditorRotateAxis;
};

#endif // CIRCLERENDERER_H
