#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Bang/Light.h"

NAMESPACE_BANG_BEGIN

class DirectionalLight : public Light
{
    COMPONENT(DirectionalLight)

public:
    // Component
    void OnRender(RenderPass rp) override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    DirectionalLight();
    virtual ~DirectionalLight();
};

NAMESPACE_BANG_END

#endif // DIRECTIONALLIGHT_H
