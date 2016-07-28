#include "Light.h"

#include "Transform.h"
#include "GameObject.h"

Light::Light()
{
}

void Light::SetUniformsBeforeApplyingLight() const
{
    ShaderProgram *sp = m_lightMaterial->GetShaderProgram();
    sp->SetUniformFloat("B_light_intensity",      m_intensity,                          false);
    sp->SetUniformVec3 ("B_light_color",          m_color,                              false);
    sp->SetUniformVec3 ("B_light_forward_world",  gameObject->transform->GetForward(),  false);
    sp->SetUniformVec3 ("B_light_position_world", gameObject->transform->GetPosition(), false);
}

void Light::ApplyLight(GBuffer *gbuffer) const
{
    SetUniformsBeforeApplyingLight();
    gbuffer->RenderPassWithMaterial(m_lightMaterial);
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

void Light::SetIntensity(float intensity)
{
    m_intensity = intensity;
}

Vector3 Light::GetColor() const
{
    return m_color;
}

float Light::GetIntensity() const
{
    return m_intensity;
}

#ifdef BANG_EDITOR
void Light::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void Light::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}
#endif

void Light::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);
    SetIntensity(xmlInfo->GetFloat("intensity"));
    SetColor(xmlInfo->GetVector3("color"));
    m_lightMaterial = AssetsManager::GetAsset<Material>(xmlInfo->GetFilepath("lightMaterial"));
}

void Light::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Light");

    xmlInfo->SetFloat("intensity", GetIntensity());
    xmlInfo->SetVector3("color", GetColor());
    xmlInfo->SetFilepath("lightMaterial", m_lightMaterial->GetFilepath());
}
