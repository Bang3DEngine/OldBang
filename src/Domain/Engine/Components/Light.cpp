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
    gbuffer->Bind();

    gbuffer->BindTexturesTo(m_material);

    m_material->Bind();
    gbuffer->RenderToScreenWithoutMaterial();
    m_material->UnBind();

    gbuffer->UnBind();
}

const std::string Light::ToString() const
{
    return "Light";
}

ICloneable *Light::Clone() const
{
    return new Light();
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
