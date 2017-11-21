#include "Bang/Light.h"

#include "Bang/Rect.h"
#include "Bang/Scene.h"
#include "Bang/XMLNode.h"
#include "Bang/Material.h"
#include "Bang/GBuffer.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

Light::Light() { }
Light::~Light() { }

void Light::SetColor(const Color &color) { m_color = color; }
void Light::SetIntensity(float intensity) { m_intensity = intensity; }

Color Light::GetColor() const { return m_color; }
float Light::GetIntensity() const { return m_intensity; }

void Light::ApplyLight(GBuffer *gbuffer, const Rect &renderRect) const
{
    m_lightMaterialScreen->Bind();
    SetUniformsBeforeApplyingLight(m_lightMaterialScreen);

    // Intersect with light rect to draw exactly what we need
    Camera *cam = SceneManager::GetActiveScene()->GetCamera();
    Rect improvedRenderRect = Rect::Intersection(GetRenderRect(cam), renderRect);
    gbuffer->ApplyPass(m_lightMaterialScreen->GetShaderProgram(),
                       true, improvedRenderRect);
    m_lightMaterialScreen->UnBind();
}

void Light::SetUniformsBeforeApplyingLight(Material *mat) const
{
    ShaderProgram *sp = mat->GetShaderProgram();
    ENSURE(sp); ASSERT(GL::IsBound(sp));

    sp->Set("B_LightIntensity", m_intensity);
    sp->Set("B_LightColor", m_color);

    Transform *tr = GetGameObject()->GetTransform();
    sp->Set("B_LightForwardWorld",  tr->GetForward());
    sp->Set("B_LightPositionWorld", tr->GetPosition());
}

void Light::SetLightMaterial(Material *lightMat)
{
    m_lightMaterialScreen = lightMat;
}

Rect Light::GetRenderRect(Camera *cam) const
{
    return Rect::ScreenRectNDC;
}

void Light::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Light *l = DCAST<Light*>(clone);
    l->SetIntensity(GetIntensity());
    l->SetColor(GetColor());
}

void Light::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("Intensity"))
    { SetIntensity(xmlInfo.Get<float>("Intensity")); }

    if (xmlInfo.Contains("Color"))
    { SetColor(xmlInfo.Get<Color>("Color")); }
}

void Light::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("Intensity", GetIntensity());
    xmlInfo->Set("Color", GetColor());
}
