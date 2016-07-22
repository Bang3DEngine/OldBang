#include "PointLight.h"

PointLight::PointLight()
{
}

const std::string PointLight::ToString() const
{
    return "PointLight";
}

std::string PointLight::GetName() const
{
    return "PointLight";
}

void PointLight::CloneInto(ICloneable *clone) const
{
    Light::CloneInto(clone);
    PointLight *pl = static_cast<PointLight*>(clone);
}

ICloneable *PointLight::Clone() const
{
    PointLight *pl = new PointLight();
    CloneInto(pl);
    return pl;
}

InspectorWidgetInfo *PointLight::GetComponentInfo()
{
    Light::GetComponentInfo();
    return &m_inspectorComponentInfo;
}

void PointLight::OnSlotValueChanged(InspectorWidget *source)
{
    Light::OnSlotValueChanged(source);
}

void PointLight::WriteInternal(std::ostream &f) const
{
    Light::WriteInternal(f);
}

void PointLight::ReadInternal(std::istream &f)
{
    Light::ReadInternal(f);
}
