#include "Bang/PointLight.h"

#include "Bang/GL.h"
#include "Bang/Rect.h"
#include "Bang/AABox.h"
#include "Bang/Gizmos.h"
#include "Bang/Sphere.h"
#include "Bang/XMLNode.h"
#include "Bang/Texture2D.h"
#include "Bang/Transform.h"
#include "Bang/IconManager.h"
#include "Bang/GameObject.h"
#include "Bang/ShaderProgram.h"
#include "Bang/MaterialFactory.h"

USING_NAMESPACE_BANG

PointLight::PointLight() : Light()
{
    SetLightMaterial(MaterialFactory::GetPointLight().Get());
}

PointLight::~PointLight()
{

}

void PointLight::SetUniformsBeforeApplyingLight(Material* mat) const
{
    Light::SetUniformsBeforeApplyingLight(mat);

    ShaderProgram *sp = mat->GetShaderProgram();
    if (!sp) { return; }
    ASSERT(GL::IsBound(sp));
    sp->Set("B_LightRange", GetRange());
}

Rect PointLight::GetRenderRect(Camera *cam) const
{
    Transform *tr = GetGameObject()->GetTransform();
    Sphere sphere(tr->GetPosition(), GetRange());
    AABox bbox = AABox::FromSphere(sphere);
    return bbox.GetAABoundingViewportRect(cam);
}

void PointLight::SetRange(float range) { m_range = range; }
float PointLight::GetRange() const { return m_range; }

void PointLight::OnRender(RenderPass rp)
{
    Component::OnRender(rp);
    if (rp == RenderPass::Overlay)
    {
        Gizmos::Reset();
        Gizmos::SetColor(GetColor());
        Gizmos::SetSelectable(GetGameObject());
        Gizmos::SetPosition( GetGameObject()->GetTransform()->GetPosition() );
        Gizmos::SetScale( Vector3(0.1f) );
        Gizmos::RenderIcon( IconManager::GetLightBulbIcon().Get(), true );
    }
}

void PointLight::CloneInto(ICloneable *clone) const
{
    Light::CloneInto(clone);
    PointLight *pl = Cast<PointLight*>(clone);
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
