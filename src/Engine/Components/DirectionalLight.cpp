#include "Bang/DirectionalLight.h"

#include "Bang/XMLNode.h"
#include "Bang/MaterialFactory.h"

USING_NAMESPACE_BANG

DirectionalLight::DirectionalLight()
{
    RH<Material> material;
    MaterialFactory::GetDirectionalLight(&material);
    SetLightMaterial(material.Get());
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
