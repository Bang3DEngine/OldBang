#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

class Camera;
class XMLNode;
class Material;
class ICloneable;
class PointLight : public Light
{
public:
    virtual String GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    ICloneable *CloneVirtual() const override;

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

    friend class GameObject;
    friend class ICloneable;
    friend class EditorFloor;
};

#endif // POINTLIGHT_H
