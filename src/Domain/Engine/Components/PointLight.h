#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

class PointLight : public Light
{
protected:
    float m_range = 1.0f;

    virtual void SetUniformsBeforeApplyingLight() const override;

public:
    PointLight();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* OnInspectorInfoNeeded() override;
    virtual void OnInspectorInfoChanged(InspectorWidgetInfo *info) override;
    #endif

    void SetRange(float range);
    float GetRange() const;

    virtual void ReadXMLNode(const XMLNode *xmlNode) override;
    virtual void GetXMLNode(XMLNode *xmlNode) const override;
};

#endif // POINTLIGHT_H
