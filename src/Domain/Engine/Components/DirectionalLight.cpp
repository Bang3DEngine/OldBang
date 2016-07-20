#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
    m_material = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/DirectionalLight.bmat");

    #ifdef BANG_EDITOR
    // m_inspectorComponentInfo.AddSlotInfo();
    #endif
}

const std::string DirectionalLight::ToString() const
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
