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
InspectorWidgetInfo *DirectionalLight::GetComponentInfo()
{
    Light::GetComponentInfo();
    return &m_inspectorComponentInfo;
}

void DirectionalLight::OnSlotValueChanged(InspectorWidget *source)
{
    Light::OnSlotValueChanged(source);
}
#endif

void DirectionalLight::WriteInternal(std::ostream &f) const
{
    Light::WriteInternal(f);
}

void DirectionalLight::ReadInternal(std::istream &f)
{
    Light::ReadInternal(f);
}
