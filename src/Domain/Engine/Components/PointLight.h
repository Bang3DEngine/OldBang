#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

class PointLight : public Light
{
protected:
    float m_range = 1.0f;

    virtual void SetUniformsBeforeApplyingLight(Material *mat) const override;

protected:
    PointLight();

public:

    virtual const String ToString() const override;
    virtual String GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnDrawGizmos() override;
    #endif

    void SetRange(float range);
    float GetRange() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    friend class GameObject;
};

#endif // POINTLIGHT_H
