#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
    m_material = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/directionalLight.bmat");

    #ifdef BANG_EDITOR
    // m_inspectorComponentInfo.AddSlotInfo();
    #endif
}

const std::string DirectionalLight::ToString() const
{
    return "DirectionalLight";
}

ICloneable *DirectionalLight::Clone() const
{
    return new DirectionalLight();
}

InspectorWidgetInfo *DirectionalLight::GetComponentInfo()
{
    Light::GetComponentInfo();
    return &m_inspectorComponentInfo;
}

void DirectionalLight::OnSlotValueChanged(InspectorWidget *source)
{

}

void DirectionalLight::Write(std::ostream &f) const
{

}

void DirectionalLight::Read(std::istream &f)
{

}
