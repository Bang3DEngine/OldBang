#include "Bang/DirectionalLight.h"

#include "Bang/Gizmos.h"
#include "Bang/XMLNode.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/Transform.h"
#include "Bang/IconManager.h"
#include "Bang/MaterialFactory.h"

USING_NAMESPACE_BANG

DirectionalLight::DirectionalLight()
{
    SetLightMaterial(MaterialFactory::GetDirectionalLight().Get());

    m_shadowMapFramebuffer = new Framebuffer(1,1);
    m_shadowMapFramebuffer->CreateAttachment(GL::Attachment::DepthStencil,
                                             GL::ColorFormat::Depth24_Stencil8);
}

DirectionalLight::~DirectionalLight()
{
    delete m_shadowMapFramebuffer;
}

void DirectionalLight::CloneInto(ICloneable *clone) const
{
    Light::CloneInto(clone);
}

void DirectionalLight::RenderShadowMaps()
{
    // Bind and resize shadow map framebuffer
    GLId prevBoundFB = GL::GetBoundId(m_shadowMapFramebuffer->GetGLBindTarget());
    m_shadowMapFramebuffer->Bind();
    m_shadowMapFramebuffer->Resize(GL::GetViewportSize().x, GL::GetViewportSize().y);

    // Render shadow map into framebuffer
    m_shadowMapFramebuffer->ClearColor(Color::One);
    // if (rand() % 10 == 0)
    {
        m_shadowMapFramebuffer->Export(GL::Attachment::DepthStencil, Path("test.png"));
    }

    // Bind previously bound framebuffer (if any)
    GL::Bind(m_shadowMapFramebuffer->GetGLBindTarget(), prevBoundFB);
}

Texture2D *DirectionalLight::GetShadowMap() const
{
    return m_shadowMapFramebuffer->GetAttachmentTexture(GL::Attachment::DepthStencil);
}

void DirectionalLight::OnRender(RenderPass rp)
{
    Component::OnRender(rp);

    // Gizmo rendering
    if (rp == RenderPass::Overlay)
    {
        Gizmos::Reset();
        Gizmos::SetColor(GetColor());
        Gizmos::SetSelectable(GetGameObject());
        Gizmos::SetPosition( GetGameObject()->GetTransform()->GetPosition() );
        Gizmos::SetScale( Vector3(0.1f) );
        Gizmos::RenderIcon( IconManager::GetSunIcon().Get(), true );
    }
}

void DirectionalLight::ImportXML(const XMLNode &xmlInfo)
{
    Light::ImportXML(xmlInfo);
}

void DirectionalLight::ExportXML(XMLNode *xmlInfo) const
{
    Light::ExportXML(xmlInfo);
}
