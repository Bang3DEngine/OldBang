#include "Bang/PointLight.h"

#include "Bang/Rect.h"
#include "Bang/AABox.h"
#include "Bang/Sphere.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/AssetsManager.h"
#include "Bang/ShaderProgram.h"

PointLight::PointLight() : Light()
{
    m_lightMaterialScreen = AssetsManager::Load<Material>(
                EPATH("Materials/SP_PointLight_Screen.bmat") );
}

PointLight::~PointLight()
{

}

void PointLight::SetUniformsBeforeApplyingLight(Material *mat) const
{
    Light::SetUniformsBeforeApplyingLight(mat);

    G_ShaderProgram *sp = mat->GetShaderProgram();
    ENSURE(sp); ASSERT(GL::IsBound(sp));
    sp->Set("B_LightRange", m_range);
}

Rect PointLight::GetRenderRect(Camera *cam) const
{
    Sphere sphere(gameObject->transform->GetPosition(), m_range);
    AABox bbox = AABox::FromSphere(sphere);
    return bbox.GetAABoundingScreenRect(cam);
}

void PointLight::SetRange(float range) { m_range = range; }
float PointLight::GetRange() const { return m_range; }

void PointLight::CloneInto(ICloneable *clone) const
{
    Light::CloneInto(clone);
    PointLight *pl = SCAST<PointLight*>(clone);
    pl->SetRange(GetRange());
}

void PointLight::Read(const XMLNode &xmlInfo)
{
    Light::Read(xmlInfo);
    SetRange(xmlInfo.Get<float>("Range"));
}

void PointLight::Write(XMLNode *xmlInfo) const
{
    Light::Write(xmlInfo);
    xmlInfo->Set("Range", m_range);
}
