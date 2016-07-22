#include "Light.h"

#include "Transform.h"
#include "GameObject.h"

Light::Light()
{
    m_color = Vector3::one;

    #ifdef BANG_EDITOR
    m_inspectorComponentInfo.AddSlotInfos(
    {
        new InspectorVFloatSWInfo("Intensity", {1.0f} )
       ,new InspectorVFloatSWInfo("Color", {1,0,0} )
    });
    #endif
}

void Light::SetUniformsBeforeApplyingLight() const
{
    ShaderProgram *sp = m_material->GetShaderProgram();
    sp->SetUniformFloat("B_light_intensity",      m_intensity,                          false);
    sp->SetUniformVec3 ("B_light_color",          m_color,                              false);
    sp->SetUniformVec3 ("B_light_forward_world",  gameObject->transform->GetForward(),  false);
    sp->SetUniformVec3 ("B_light_position_world", gameObject->transform->GetPosition(), false);
}

void Light::ApplyLight(GBuffer *gbuffer) const
{
    SetUniformsBeforeApplyingLight();
    gbuffer->RenderPassWithMaterial(m_material);
}

const std::string Light::ToString() const
{
    return "Light";
}

void Light::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Light *l = static_cast<Light*>(clone);
    l->SetColor(GetColor());
}

ICloneable *Light::Clone() const
{
    Light *l = new Light();
    CloneInto(l);
    return l;
}

void Light::SetColor(const Vector3 &color)
{
    m_color = color;
}

Vector3 Light::GetColor() const
{
    return m_color;
}

#ifdef BANG_EDITOR
InspectorWidgetInfo *Light::GetComponentInfo()
{
    Component::GetComponentInfo();

    static_cast<InspectorVFloatSWInfo*>(m_inspectorComponentInfo.GetSlotInfo(0))->m_value =
        {m_intensity};

    static_cast<InspectorVFloatSWInfo*>(m_inspectorComponentInfo.GetSlotInfo(1))->m_value =
        {m_color.r, m_color.g, m_color.b};

    return &m_inspectorComponentInfo;
}

void Light::OnSlotValueChanged(InspectorWidget *source)
{
    std::vector<float> intensity = source->GetSWVectorFloatValue("Intensity");
    m_intensity = intensity[0];

    std::vector<float> color = source->GetSWVectorFloatValue("Color");
    m_color = Vector3(color[0], color[1], color[2]);
}
#endif

void Light::WriteInternal(std::ostream &f) const
{
    Component::WriteInternal(f);
    FileWriter::WriteFloat(m_intensity, f);
    FileWriter::WriteVector3(m_color, f);
    FileWriter::WriteFilepath(m_material->GetFilepath(), f);
}

void Light::ReadInternal(std::istream &f)
{
    Component::ReadInternal(f);
    m_intensity = FileReader::ReadFloat(f);
    m_color = FileReader::ReadVec3(f);

    std::string materialFilepath = FileReader::ReadString(f);
    m_material = AssetsManager::GetAsset<Material>(materialFilepath);

}
