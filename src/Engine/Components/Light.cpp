#include "Bang/Light.h"

#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Gizmos.h"
#include "Bang/XMLNode.h"
#include "Bang/Renderer.h"
#include "Bang/Material.h"
#include "Bang/G_GBuffer.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"

Light::Light()
{
}

Light::~Light()
{

}

void Light::SetUniformsBeforeApplyingLight(Material *mat) const
{
    G_ShaderProgram *sp = mat->GetShaderProgram();
    ENSURE(sp); ASSERT(GL::IsBound(sp));

    sp->SetFloat("B_LightIntensity", m_intensity);
    sp->SetColor("B_LightColor", m_color);

    Transform *t = gameObject->transform;
    sp->SetVec3("B_LightForwardWorld",  t->GetForward());
    sp->SetVec3("B_LightPositionWorld", t->GetPosition());
}

void Light::ApplyLight(G_GBuffer *gbuffer, const Rect &renderRect) const
{
    m_lightMaterialScreen->Bind();
    SetUniformsBeforeApplyingLight(m_lightMaterialScreen);

    // Intersect with light rect to draw exactly what we need
    Camera *cam = SceneManager::GetActiveScene()->GetCamera();
    Rect improvedRenderRect = Rect::Intersection(GetRenderRect(cam), renderRect);
    gbuffer->ApplyPass(m_lightMaterialScreen->GetShaderProgram(),
                       true,
                       improvedRenderRect);
    m_lightMaterialScreen->UnBind();
}

Rect Light::GetRenderRect(Camera *cam) const
{
    // Well implemented for each kind of light
    return Rect::ScreenRect;
}

void Light::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Light *l = Object::SCast<Light>(clone);
    l->SetIntensity(GetIntensity());
    l->SetColor(GetColor());
}

void Light::SetColor(const Color &color)
{
    m_color = color;
}

void Light::SetIntensity(float intensity)
{
    m_intensity = intensity;
}

Color Light::GetColor() const
{
    return m_color;
}

float Light::GetIntensity() const
{
    return m_intensity;
}

void Light::Read(const XMLNode &xmlInfo)
{
    Component::Read(xmlInfo);
    SetIntensity(xmlInfo.GetFloat("Intensity"));
    SetColor(xmlInfo.GetColor("Color"));
}

void Light::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);

    xmlInfo->SetFloat("Intensity", GetIntensity());
    xmlInfo->SetColor("Color", GetColor());
}
