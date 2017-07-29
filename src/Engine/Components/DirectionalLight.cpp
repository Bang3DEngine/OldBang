#include "Bang/DirectionalLight.h"

#include "Bang/Material.h"
#include "Bang/AssetsManager.h"

DirectionalLight::DirectionalLight()
{
    m_lightMaterialScreen = AssetsManager::Load<Material>(
                EPATH("Materials/SP_DirectionalLight_Screen.bmat") );
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::CloneInto(ICloneable *clone) const
{
    Light::CloneInto(clone);
    DirectionalLight *dl = SCAST<DirectionalLight*>(clone);
}

void DirectionalLight::Read(const XMLNode &xmlInfo)
{
    Light::Read(xmlInfo);
}

void DirectionalLight::Write(XMLNode *xmlInfo) const
{
    Light::Write(xmlInfo);
}
