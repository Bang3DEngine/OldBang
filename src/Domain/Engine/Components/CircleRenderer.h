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
    virtual ICloneable *Clone() const override;

    void SetRadius(float m_radius);
    void SetSegments(int m_segments);

    float GetRadius() const;
    int GetSegments() const;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void Read(const XMLNode *xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    CircleRenderer();

private:
    float m_radius = 1.0f;
    int m_segments = 32;

    void GeneratePoints();

    // Used by EditorRotateAxis mainly
    void GetTwoClosestPointsInScreenSpace(
            const Vector2 &sOrigin,
            Vector2 *p0, int *i0,
            Vector2 *p1, int *i1 ) const;

    friend class GameObject;
    friend class EditorRotateAxis;
};

#endif // CIRCLERENDERER_H
