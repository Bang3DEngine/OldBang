#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
    material = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/directionalLight.bmat");
    color = Vector3::one;

    #ifdef BANG_EDITOR
    m_inspectorComponentInfo.SetSlotsInfos(
    {
        new InspectorVFloatSWInfo( "Color", {1,1,1} )
    });
    #endif
}

void DirectionalLight::ApplyLight(GBuffer *gbuffer)
{
    gbuffer->Bind();

    gbuffer->BindTexturesTo(material);

    material->Bind();
    gbuffer->RenderToScreenWithoutMaterial();
    material->UnBind();

    gbuffer->UnBind();
}

const std::string DirectionalLight::ToString() const
{
    return "DirectionalLight";
}

ICloneable *DirectionalLight::Clone() const
{
    return new DirectionalLight();
}

InspectorWidgetInfo *DirectionalLight::GetComponentInfo()
{
    Component::GetComponentInfo();
    return &m_inspectorComponentInfo;
}

void DirectionalLight::OnSlotValueChanged(InspectorWidget *source)
{

}

void DirectionalLight::Write(std::ostream &f) const
{

}

void DirectionalLight::Read(std::istream &f)
{

}
