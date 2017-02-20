#include "Light.h"

#include "Scene.h"
#include "Gizmos.h"
#include "XMLNode.h"
#include "GBuffer.h"
#include "Renderer.h"
#include "Material.h"
#include "Transform.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "ShaderProgram.h"

#ifdef BANG_EDITOR
#include "InspectorWidget.h"
#endif

Light::Light()
{
}

Light::~Light()
{

}

void Light::SetUniformsBeforeApplyingLight(Material *mat) const
{
    ShaderProgram *sp = mat->GetShaderProgram();
    sp->SetUniformFloat("B_light_intensity",      m_intensity                         );
    sp->SetUniformColor("B_light_color",          m_color                             );
    sp->SetUniformVec3 ("B_light_forward_world",  gameObject->transform->GetForward() );
    sp->SetUniformVec3 ("B_light_position_world", gameObject->transform->GetPosition());
}

void Light::ApplyLight(GBuffer *gbuffer, const Rect &renderRect) const
{
    gbuffer->Bind();

    SetUniformsBeforeApplyingLight(m_lightMaterialScreen);

    // Intersect with light rect to draw exactly what we need
    Camera *cam = SceneManager::GetActiveScene()->GetCamera();
    Rect improvedRenderRect = renderRect; //Rect::Intersection(GetRenderRect(cam), renderRect);
    gbuffer->RenderPassWithMaterial(m_lightMaterialScreen, improvedRenderRect);

    gbuffer->UnBind();
}

Rect Light::GetRenderRect(Camera *cam) const
{
    return Rect::ScreenRect; // By default, no rect rendering constraint
}

String Light::GetName() const
{
    return "Light";
}

void Light::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Light *l = static_cast<Light*>(clone);
    l->SetIntensity(GetIntensity());
    l->SetColor(GetColor());
}

ICloneable *Light::Clone() const
{
    Light *l = new Light();
    CloneInto(l);
    return l;
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

void Light::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);
    SetIntensity(xmlInfo->GetFloat("Intensity"));
    SetColor(xmlInfo->GetColor("Color"));
}

void Light::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Light");

    xmlInfo->SetFloat("Intensity", GetIntensity());
    xmlInfo->SetColor("Color", GetColor());
}
