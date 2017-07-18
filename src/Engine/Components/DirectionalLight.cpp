#include "Bang/DirectionalLight.h"

#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Material.h"
#include "Bang/Texture2D.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
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
    DirectionalLight *dl = Object::SCast<DirectionalLight>(clone);
}

void DirectionalLight::Read(const XMLNode &xmlInfo)
{
    Light::Read(xmlInfo);
}

void DirectionalLight::Write(XMLNode *xmlInfo) const
{
    Light::Write(xmlInfo);
}
