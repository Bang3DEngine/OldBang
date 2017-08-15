#include "Bang/DirectionalLight.h"

#include "Bang/XMLNode.h"
#include "Bang/MaterialFactory.h"

DirectionalLight::DirectionalLight()
{
    SetLightMaterial( MaterialFactory::GetDirectionalLight() );
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::CloneInto(ICloneable *clone) const
{
    Light::CloneInto(clone);
}

void DirectionalLight::Read(const XMLNode &xmlInfo)
{
    Light::Read(xmlInfo);
}

void DirectionalLight::Write(XMLNode *xmlInfo) const
{
    Light::Write(xmlInfo);
}
