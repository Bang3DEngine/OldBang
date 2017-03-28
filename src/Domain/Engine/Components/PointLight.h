#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Bang/Light.h"

class Camera;
class XMLNode;
class Material;
class ICloneable;
class PointLight : public Light
{
    OBJECT(PointLight)
    ICLONEABLE(PointLight)
    COMPONENT_ICON(PointLight, "Icons/PointLightIcon.png")

public:
    virtual void CloneInto(ICloneable *clone) const override;

    void SetRange(float range);
    float GetRange() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    virtual void OnDrawGizmos(bool depthed, bool overlay) override;

protected:
    float m_range = 1.0f;

    PointLight();
    virtual ~PointLight();

    Rect GetRenderRect(Camera *cam) const override;
    virtual void SetUniformsBeforeApplyingLight(Material *mat) const override;

    friend class EditorFloor;
};

#endif // POINTLIGHT_H
