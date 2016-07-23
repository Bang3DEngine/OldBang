#include "PointLight.h"

PointLight::PointLight() : Light()
{
    m_lightMaterial = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/PR_PointLight.bmat");

    #ifdef BANG_EDITOR
    m_inspectorComponentInfo.AddSlotInfos(
    {
        new InspectorVFloatSWInfo("Range", {m_range} )
    });
    #endif
}


void PointLight::SetUniformsBeforeApplyingLight() const
{
    Light::SetUniformsBeforeApplyingLight();

    ShaderProgram *sp = m_lightMaterial->GetShaderProgram();
    sp->SetUniformFloat("B_light_range", m_range, false);
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
    pl->SetRange(GetRange());
}

ICloneable *PointLight::Clone() const
{
    PointLight *pl = new PointLight();
    CloneInto(pl);
    return pl;
}

#ifdef BANG_EDITOR
InspectorWidgetInfo *PointLight::GetComponentInfo()
{
    Light::GetComponentInfo();

    static_cast<InspectorVFloatSWInfo*>(m_inspectorComponentInfo.GetSlotInfo(2))->m_value =
        {m_range};

    return &m_inspectorComponentInfo;
}

void PointLight::OnSlotValueChanged(InspectorWidget *source)
{
    Light::OnSlotValueChanged(source);

    std::vector<float> range = source->GetSWVectorFloatValue("Range");
    m_range = range[0];

}
#endif

void PointLight::SetRange(float range)
{
    m_range = range;
}

float PointLight::GetRange() const
{
    return m_range;
}

void PointLight::WriteInternal(std::ostream &f) const
{
    Light::WriteInternal(f);
    FileWriter::WriteFloat(m_range, f);
}

void PointLight::ReadInternal(std::istream &f)
{
    Light::ReadInternal(f);
    m_range = FileReader::ReadFloat(f);
}
