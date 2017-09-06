#include "Bang/PointLight.h"

#include "Bang/Rect.h"
#include "Bang/AABox.h"
#include "Bang/Sphere.h"
#include "Bang/XMLNode.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/ShaderProgram.h"
#include "Bang/MaterialFactory.h"

USING_NAMESPACE_BANG

PointLight::PointLight() : Light()
{
    SetLightMaterial( MaterialFactory::GetPointLight() );
}

PointLight::~PointLight()
{

}

void PointLight::SetUniformsBeforeApplyingLight(Material *mat) const
{
    Light::SetUniformsBeforeApplyingLight(mat);

    ShaderProgram *sp = mat->GetShaderProgram();
    ENSURE(sp); ASSERT(GL::IsBound(sp));
    sp->Set("B_LightRange", GetRange());
}

Rect PointLight::GetRenderRect(Camera *cam) const
{
    Sphere sphere(gameObject->transform->GetPosition(), GetRange());
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

void PointLight::ImportXML(const XMLNode &xmlInfo)
{
    Light::ImportXML(xmlInfo);
    if (xmlInfo.Contains("Range"))
    { SetRange(xmlInfo.Get<float>("Range")); }
}

void PointLight::ExportXML(XMLNode *xmlInfo) const
{
    Light::ExportXML(xmlInfo);
    xmlInfo->Set("Range", GetRange());
}


