#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
    m_lightMaterial = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/PR_DirectionalLight.bmat");

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
InspectorWidgetInfo *DirectionalLight::OnInspectorInfoNeeded()
{
    Light::OnInspectorInfoNeeded();
    return &m_inspectorInfo;
}

void DirectionalLight::OnInspectorInfoChanged(InspectorWidgetInfo *info)
{
    Light::OnInspectorInfoChanged(info);
}
#endif


void DirectionalLight::ReadXMLNode(const XMLNode *xmlNode)
{
    Light::ReadXMLNode(xmlNode);
}

void DirectionalLight::GetXMLNode(XMLNode *xmlNode) const
{
    Light::GetXMLNode(xmlNode);
    xmlNode->SetTagName("DirectionalLight");
}
