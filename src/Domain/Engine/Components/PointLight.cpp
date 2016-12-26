#include "PointLight.h"

#include "Gizmos.h"
#include "Material.h"
#include "Texture2D.h"
#include "Transform.h"
#include "GameObject.h"
#include "AssetsManager.h"
#include "ShaderProgram.h"

#ifdef BANG_EDITOR
#include "InspectorWidget.h"
#endif


PointLight::PointLight() : Light()
{
    m_lightMaterialScreen = AssetsManager::LoadAsset<Material>(
                "Assets/Engine/Materials/PR_PointLight_Screen.bmat");
}

Rect PointLight::GetRenderRect(Camera *cam) const
{
    Sphere sphere(gameObject->transform->GetPosition(), m_range);
    Box bbox = Box::FromSphere(sphere);
    return bbox.GetBoundingScreenRect(cam);
}


void PointLight::SetUniformsBeforeApplyingLight(Material *mat) const
{
    Light::SetUniformsBeforeApplyingLight(mat);

    ShaderProgram *sp = mat->GetShaderProgram();
    sp->SetUniformFloat("B_light_range", m_range, false);
}


const String PointLight::ToString() const
{
    return "PointLight";
}

String PointLight::GetName() const
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
    Gizmos::SetScale(Vector3::One * 0.1f);
    Gizmos::RenderIcon(tex);

    if (gameObject->IsSelected())
    {
        Gizmos::SetReceivesLighting(false);
        Gizmos::SetColor(Color(GetColor(), 1));
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
