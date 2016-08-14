#include "PointLight.h"

#include "Transform.h"
#include "GameObject.h"

PointLight::PointLight() : Light()
{
    m_lightMaterial = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/PR_PointLight.bmat");
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
void PointLight::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void PointLight::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}

void PointLight::OnDrawGizmos()
{
    Light::OnDrawGizmos();

    Texture2D *tex = AssetsManager::LoadAsset<Texture2D>("./Assets/Engine/Textures/PointLightIcon.btex2d");
    Gizmos::SetColor(Color(GetColor(), 1));
    Gizmos::SetPosition(transform->GetPosition());
    Gizmos::SetScale(Vector3::one * 10.0f);
    Gizmos::RenderIcon(tex);

    if (gameObject->IsSelectedInHierarchy())
    {
        Gizmos::SetDrawWireframe(true);
        Gizmos::SetReceivesLighting(false);
        Gizmos::RenderSimpleSphere(transform->GetPosition(), GetRange());
    }
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

void PointLight::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Light::ReadXMLInfo(xmlInfo);
    SetRange(xmlInfo->GetFloat("Range"));
}

void PointLight::FillXMLInfo(XMLNode *xmlInfo) const
{
    Light::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("PointLight");

    xmlInfo->SetFloat("Range", m_range);
}
