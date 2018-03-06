#include "Bang/Light.h"

#include "Bang/GL.h"
#include "Bang/Scene.h"
#include "Bang/AARect.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/XMLNode.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

Light::Light() { }
Light::~Light() { }

void Light::SetColor(const Color &color) { m_color = color; }
void Light::SetIntensity(float intensity) { m_intensity = intensity; }
void Light::SetShadowBias(float shadowBias) { m_shadowBias = shadowBias; }
void Light::SetShadowType(ShadowType shadowType) { m_shadowType = shadowType; }

Color Light::GetColor() const { return m_color; }
float Light::GetIntensity() const { return m_intensity; }
float Light::GetShadowBias() const { return m_shadowBias; }
Light::ShadowType Light::GetShadowType() const { return m_shadowType; }

void Light::RenderShadowMaps()
{
    if (GetShadowType() != ShadowType::NONE)
    {
        RenderShadowMaps_();
    }
}

void Light::ApplyLight(Camera *camera, const AARect &renderRect) const
{
    p_lightMaterial.Get()->Bind();
    SetUniformsBeforeApplyingLight(p_lightMaterial.Get());

    // Intersect with light rect to draw exactly what we need
    GBuffer *gbuffer = camera->GetGBuffer();
    AARect improvedRenderRect = AARect::Intersection(GetRenderRect(camera),
                                                 renderRect);
    gbuffer->ApplyPass(p_lightMaterial.Get()->GetShaderProgram(),
                       true, improvedRenderRect);
    p_lightMaterial.Get()->UnBind();
}

void Light::SetUniformsBeforeApplyingLight(Material* mat) const
{
    ShaderProgram *sp = mat->GetShaderProgram();
    if (!sp) { return; }

    ASSERT(GL::IsBound(sp))

    sp->Set("B_ShadowType", SCAST<int>( GetShadowType() ), true);
    sp->Set("B_LightShadowBias", GetShadowBias(), true);
    sp->Set("B_LightIntensity", GetIntensity(), false);
    sp->Set("B_LightColor", GetColor(), false);

    Transform *tr = GetGameObject()->GetTransform();
    sp->Set("B_LightForwardWorld",  tr->GetForward(), false);
    sp->Set("B_LightPositionWorld", tr->GetPosition(), false);
}

void Light::SetLightMaterial(Material *lightMat)
{
    p_lightMaterial.Set(lightMat);
}

AARect Light::GetRenderRect(Camera *camera) const
{
    return AARect::NDCRect;
}

void Light::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Light *l = Cast<Light*>(clone);
    l->SetIntensity(GetIntensity());
    l->SetColor(GetColor());
    l->SetShadowBias( GetShadowBias() );
    l->SetShadowType( GetShadowType() );
}

void Light::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("Intensity"))
    { SetIntensity(xmlInfo.Get<float>("Intensity")); }

    if (xmlInfo.Contains("Color"))
    { SetColor(xmlInfo.Get<Color>("Color")); }

    if (xmlInfo.Contains("ShadowBias"))
    { SetShadowBias(xmlInfo.Get<float>("ShadowBias")); }

    if (xmlInfo.Contains("ShadowType"))
    { SetShadowType(xmlInfo.Get<ShadowType>("ShadowType")); }
}

void Light::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("Intensity", GetIntensity());
    xmlInfo->Set("Color", GetColor());
    xmlInfo->Set("ShadowBias", GetShadowBias());
    xmlInfo->Set("ShadowType", GetShadowType());
}
