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

void DirectionalLight::ImportXML(const XMLNode &xmlInfo)
{
    Light::ImportXML(xmlInfo);
}

void DirectionalLight::ExportXML(XMLNode *xmlInfo) const
{
    Light::ExportXML(xmlInfo);
}
