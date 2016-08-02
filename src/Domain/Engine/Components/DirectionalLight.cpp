#include "DirectionalLight.h"

#include "Transform.h"
#include "GameObject.h"

DirectionalLight::DirectionalLight() : Light()
{
    m_lightMaterial = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/PR_DirectionalLight.bmat");

    #ifdef BANG_EDITOR
    // m_inspectorComponentInfo.AddSlotInfos();
    #endif
}

const std::string DirectionalLight::ToString() const
{
    return "DirectionalLight";
}

std::string DirectionalLight::GetName() const
{
    return "DirectionalLight";
}

void DirectionalLight::CloneInto(ICloneable *clone) const
{
    Light::CloneInto(clone);
    DirectionalLight *dl = static_cast<DirectionalLight*>(clone);
}

ICloneable *DirectionalLight::Clone() const
{
    DirectionalLight *dl = new DirectionalLight();
    CloneInto(dl);
    return dl;
}

#ifdef BANG_EDITOR
void DirectionalLight::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void DirectionalLight::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}

void DirectionalLight::OnDrawGizmos()
{
    Light::OnDrawGizmos();

    Gizmos::SetColor(glm::vec4(0));
    Texture2D *tex = AssetsManager::LoadAsset<Texture2D>("Assets/Engine/Textures/DirectionalLightIcon.btex2d");
    Vector3 c = GetColor();
    Gizmos::SetColor(glm::vec4(c.r, c.g, c.b, 0.5f));
    Gizmos::DrawIcon(tex, gameObject->transform->GetPosition(), Vector3::one * 5.0f);
}
#endif


void DirectionalLight::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Light::ReadXMLInfo(xmlInfo);
}

void DirectionalLight::FillXMLInfo(XMLNode *xmlInfo) const
{
    Light::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("DirectionalLight");
}
