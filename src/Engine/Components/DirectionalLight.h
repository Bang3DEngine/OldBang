#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Bang/Light.h"

class DirectionalLight : public Light
{
    OBJECT(DirectionalLight)

public:
    virtual void CloneInto(ICloneable *clone) const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    DirectionalLight();
    virtual ~DirectionalLight();
};

#endif // DIRECTIONALLIGHT_H
