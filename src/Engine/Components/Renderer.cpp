#include "Bang/Renderer.h"

#include <functional>

#include "Bang/GL.h"
#include "Bang/AABox.h"
#include "Bang/AARect.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Transform.h"
#include "Bang/Resources.h"
#include "Bang/GLUniforms.h"
#include "Bang/GameObject.h"
#include "Bang/ShaderProgram.h"
#include "Bang/MaterialFactory.h"

USING_NAMESPACE_BANG

Renderer::Renderer()
{
    SetMaterial(MaterialFactory::GetDefault().Get());
}

Renderer::~Renderer()
{
}

Material *Renderer::GetActiveMaterial() const
{
    if (p_material) { return GetMaterial(); }
    return GetSharedMaterial();
}

void Renderer::OnRender(RenderPass renderPass)
{
    Component::OnRender(renderPass);

    Material *mat = GetActiveMaterial();
    if (IsVisible() && mat && mat->GetRenderPass() == renderPass)
    {
        GEngine::GetActive()->Render(this);
    }
}
void Renderer::OnRender() {}

void Renderer::Bind() const
{
    GL::SetViewProjMode( GetViewProjMode() );
    GL::SetWireframe( IsRenderWireframe() );

    GL::SetCullFace( GetCullFace() ); // Culling states
    GL::SetEnabled(GL::Test::CullFace, GetCulling());

    GL::LineWidth( GetLineWidth() );
    GL::PointSize( GetLineWidth() );

    Transform *t = GetGameObject()->GetTransform();
    GLUniforms::SetModelMatrix( t ? t->GetLocalToWorldMatrix() : Matrix4::Identity );

    if (GetActiveMaterial()) { GetActiveMaterial()->Bind(); }
}

void Renderer::UnBind() const
{
    if (GetActiveMaterial()) { GetActiveMaterial()->UnBind(); }
}


void Renderer::SetVisible(bool visible)
{
    if (visible != IsVisible())
    {
        m_visible = visible;
        PropagateRendererChanged();
    }
}


void Renderer::SetMaterial(Material *mat)
{
    if (GetSharedMaterial() != mat)
    {
        if (GetSharedMaterial())
        {
            GetSharedMaterial()->EventEmitter<IMaterialChangedListener>::
                                 UnRegisterListener(this);
        }

        if (p_material.Get())
        {
            p_material.Get()->EventEmitter<IMaterialChangedListener>::
                              UnRegisterListener(this);
        }

        p_sharedMaterial.Set(mat);
        p_material.Set(nullptr);

        if (GetSharedMaterial())
        {
            GetSharedMaterial()->EventEmitter<IMaterialChangedListener>::
                                 RegisterListener(this);
        }

        PropagateRendererChanged();
    }
}

void Renderer::SetLineWidth(float w)
{
    if (w != GetLineWidth())
    {
        m_lineWidth = w;
        PropagateRendererChanged();
    }
}
void Renderer::SetRenderWireframe(bool renderWireframe)
{
    if (renderWireframe != IsRenderWireframe())
    {
        m_renderWireframe = renderWireframe;
        PropagateRendererChanged();
    }
}
void Renderer::SetViewProjMode(GL::ViewProjMode viewProjMode)
{
    if (viewProjMode != GetViewProjMode())
    {
        m_viewProjMode = viewProjMode;
        PropagateRendererChanged();
    }
}
void Renderer::SetRenderPrimitive(GL::Primitive renderPrimitive)
{
    if (renderPrimitive != GetRenderPrimitive())
    {
        m_renderPrimitive = renderPrimitive;
        PropagateRendererChanged();
    }
}

bool Renderer::IsVisible() const { return m_visible; }
Material* Renderer::GetSharedMaterial() const { return p_sharedMaterial.Get(); }

void Renderer::OnMaterialChanged(const Material*) { PropagateRendererChanged(); }
bool Renderer::IsRenderWireframe() const { return m_renderWireframe; }
AABox Renderer::GetAABBox() const { return AABox::Empty; }
void Renderer::SetCullFace(GL::Face cullMode) { m_cullFace = cullMode; }
void Renderer::SetCulling(bool culling) { m_cullling = culling; }
GL::Face Renderer::GetCullFace() const { return m_cullFace; }
bool Renderer::GetCulling() const { return m_cullling; }
GL::ViewProjMode Renderer::GetViewProjMode() const { return m_viewProjMode; }
GL::Primitive Renderer::GetRenderPrimitive() const { return m_renderPrimitive; }
float Renderer::GetLineWidth() const { return m_lineWidth; }
Material* Renderer::GetMaterial() const
{
    if (!p_material)
    {
        if (GetSharedMaterial())
        {
            p_material = Resources::Clone<Material>(GetSharedMaterial());
            p_material.Get()->EventEmitter<IMaterialChangedListener>::
                              RegisterListener(const_cast<Renderer*>(this));
        }
    }
    return p_material.Get();
}

AARect Renderer::GetBoundingRect(Camera *camera) const
{
    return AARect::NDCRect;
    return camera ? camera->GetViewportBoundingRect(GetAABBox()) :
                    AARect::Zero;
}

void Renderer::PropagateRendererChanged()
{
    EventEmitter<IRendererChangedListener>::PropagateToListeners(
                &IRendererChangedListener::OnRendererChanged, this);
}

void Renderer::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Renderer *r = Cast<Renderer*>(clone);
    r->SetMaterial(GetSharedMaterial());
    r->SetRenderWireframe(IsRenderWireframe());
    r->SetCullFace(GetCullFace());
    r->SetCulling(GetCulling());
    r->SetRenderPrimitive(GetRenderPrimitive());
    r->SetLineWidth(GetLineWidth());
}

void Renderer::ImportXML(const XMLNode &xml)
{
    Component::ImportXML(xml);

    if (xml.Contains("Visible"))
    { SetVisible( xml.Get<bool>("Visible") ); }

    if (xml.Contains("Material"))
    { SetMaterial(Resources::Load<Material>(xml.Get<GUID>("Material")).Get()); }

    if (xml.Contains("LineWidth"))
    { SetLineWidth(xml.Get<float>("LineWidth")); }

    if (xml.Contains("RenderWireframe"))
    { SetRenderWireframe(xml.Get<bool>("RenderWireframe")); }
}

void Renderer::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("Visible", IsVisible());

    Material* sMat = GetSharedMaterial();
    xmlInfo->Set("Material", sMat ? sMat->GetGUID() : GUID::Empty());

    xmlInfo->Set("LineWidth", GetLineWidth());
    xmlInfo->Set("RenderWireframe", IsRenderWireframe());
}
