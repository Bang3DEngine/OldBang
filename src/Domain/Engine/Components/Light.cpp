#include "Light.h"

Light::Light()
{
    m_color = Vector3::one;

    #ifdef BANG_EDITOR
    m_inspectorComponentInfo.SetSlotsInfos(
    {
        new InspectorVFloatSWInfo( "Color", {1,1,1} )
    });
    #endif
}

void Light::ApplyLight(GBuffer *gbuffer) const
{
    //gbuffer->Bind();

    //gbuffer->BindTexturesTo(m_material);

    gbuffer->RenderToScreenWithMaterial(m_material);
    //gbuffer->RenderToScreen();

    //gbuffer->UnBind();
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

Vector3 Light::GetColor() const
{
    return m_color;
}

InspectorWidgetInfo *Light::GetComponentInfo()
{
    Component::GetComponentInfo();
    return &m_inspectorComponentInfo;
}

void Light::OnSlotValueChanged(InspectorWidget *source)
{

}

void Light::Write(std::ostream &f) const
{

}

void Light::Read(std::istream &f)
{

}
