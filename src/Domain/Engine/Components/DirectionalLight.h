#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.h"

class DirectionalLight : public Light
{

public:
    DirectionalLight();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* OnInspectorInfoNeeded() override;
    virtual void OnInspectorInfoChanged(InspectorWidgetInfo *info) override;
    #endif

    virtual void ReadXMLNode(const XMLNode *xmlNode) override;
    virtual void GetXMLNode(XMLNode *xmlNode) const override;
};

#endif // DIRECTIONALLIGHT_H
