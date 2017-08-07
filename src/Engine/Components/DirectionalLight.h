#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Bang/Light.h"

class DirectionalLight : public Light
{
    COMPONENT(DirectionalLight)

public:
    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    DirectionalLight();
    virtual ~DirectionalLight();
};

#endif // DIRECTIONALLIGHT_H
