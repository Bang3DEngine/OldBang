#include "Bang/PointLight.h"

#include "Bang/Gizmos.h"
#include "Bang/Material.h"
#include "Bang/Texture2D.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/AssetsManager.h"
#include "Bang/ShaderProgram.h"

#ifdef BANG_EDITOR
#include "Bang/InspectorWidget.h"
#endif


PointLight::PointLight() : Light()
{
    m_lightMaterialScreen = AssetsManager::Load<Material>(
                "Materials/SP_PointLight_Screen.bmat", true);
}

PointLight::~PointLight()
{

}

Rect PointLight::GetRenderRect(Camera *cam) const
{
    Sphere sphere(gameObject->transform->GetPosition(), m_range);
    AABox bbox = AABox::FromSphere(sphere);
    return bbox.GetAABoundingScreenRect(cam);;
}


void PointLight::SetUniformsBeforeApplyingLight(Material *mat) const
{
    Light::SetUniformsBeforeApplyingLight(mat);

    ShaderProgram *sp = mat->GetShaderProgram();
    sp->SetFloat("B_LightRange", m_range);
}

void PointLight::CloneInto(ICloneable *clone) const
{
    Light::CloneInto(clone);
    PointLight *pl = Object::SCast<PointLight>(clone);
    pl->SetRange(GetRange());
}

void PointLight::SetRange(float range)
{
    m_range = range;
}

float PointLight::GetRange() const
{
    return m_range;
}

void PointLight::Read(const XMLNode &xmlInfo)
{
    Light::Read(xmlInfo);
    SetRange(xmlInfo.GetFloat("Range"));
}

void PointLight::Write(XMLNode *xmlInfo) const
{
    Light::Write(xmlInfo);
    xmlInfo->SetFloat("Range", m_range);
}

void PointLight::OnDrawGizmos(bool depthed, bool overlay)
{
    Light::OnDrawGizmos(depthed, overlay);

    if (!depthed && !overlay)
    {
        Texture2D *tex = AssetsManager::Load<Texture2D>(
                    "Textures/PointLightIcon.btex2d", true);
        Gizmos::SetColor(Color(GetColor(), 1));
        Gizmos::SetPosition(transform->GetPosition());
        Gizmos::SetScale(Vector3::One * 0.1f);
        Gizmos::RenderIcon(tex);
    }

    if (depthed && gameObject->IsSelected())
    {
        Gizmos::SetReceivesLighting(false);
        Gizmos::SetColor(Color(GetColor(), 1));
        Gizmos::RenderSimpleSphere(transform->GetPosition(), GetRange());
    }
}
