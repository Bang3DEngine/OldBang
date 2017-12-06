#include "Bang/Renderer.h"

#include <functional>

#include "Bang/Rect.h"
#include "Bang/AABox.h"
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
    RH<Material> material;
    MaterialFactory::GetDefault(&material);
    SetMaterial(material.Get());
}

Renderer::~Renderer()
{
}

void Renderer::OnRender(RenderPass renderPass)
{
    Component::OnRender(renderPass);
    if (IsVisible() && renderPass == GetRenderPass())
    {
        GEngine::GetActive()->Render(this);
    }
}
void Renderer::OnRender() { }

void Renderer::Bind() const
{
    Transform *t = GetGameObject()->GetTransform();
    if (t) { GLUniforms::SetModelMatrix(t->GetLocalToWorldMatrix()); }

    GL::SetViewProjMode( GetViewProjMode() );
    GL::SetWireframe( IsRenderWireframe() );
    GL::SetCullFace( GetCullMode() );
    GL::LineWidth( GetLineWidth() );

    GetMaterial()->Bind();
}

void Renderer::UnBind() const
{
    GetMaterial()->UnBind();
}

void Renderer::SetVisible(bool visible)
{
    m_visible = visible;
}


void Renderer::SetMaterial(Material *mat)
{
    if (GetMaterial() != mat)
    {
        p_material.Set(mat);
    }
}

void Renderer::SetRenderPass(RenderPass rp) { m_renderPass = rp; }

bool Renderer::IsVisible() const { return m_visible; }

RenderPass Renderer::GetRenderPass() const { return m_renderPass; }
bool Renderer::IsRenderWireframe() const { return m_drawWireframe; }
AABox Renderer::GetAABBox() const { return AABox(); }
void Renderer::SetCullMode(GL::Face cullMode) { m_cullMode = cullMode; }
GL::Face Renderer::GetCullMode() const { return m_cullMode; }
void Renderer::SetLineWidth(float w) { m_lineWidth = w; }
void Renderer::SetRenderWireframe(bool drawWireframe)
{
    m_drawWireframe = drawWireframe;
}
void Renderer::SetViewProjMode(GL::ViewProjMode viewProjMode)
{
    m_viewProjMode = viewProjMode;
}
void Renderer::SetRenderPrimitive(GL::Primitives renderMode)
{
    m_renderMode = renderMode;
}

GL::ViewProjMode Renderer::GetViewProjMode() const { return m_viewProjMode; }
GL::Primitives Renderer::GetRenderPrimitive() const { return m_renderMode; }
float Renderer::GetLineWidth() const { return m_lineWidth; }
Material* Renderer::GetMaterial() const { return p_material.Get(); }

Rect Renderer::GetBoundingRect(Camera *camera) const
{
    return Rect::ScreenRectNDC;
    return camera ? camera->GetScreenBoundingRect(GetAABBox()) :
                    Rect::Zero;
}

void Renderer::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Renderer *r = Cast<Renderer*>(clone);
    r->SetMaterial(GetMaterial());
    r->SetRenderWireframe(IsRenderWireframe());
    r->SetCullMode(GetCullMode());
    r->SetRenderPrimitive(GetRenderPrimitive());
    r->SetLineWidth(GetLineWidth());
}

void Renderer::ImportXML(const XMLNode &xml)
{
    Component::ImportXML(xml);

    if (xml.Contains("Visible"))
    { SetVisible( xml.Get<bool>("Visible") ); }

    if (xml.Contains("Material"))
    {
        RH<Material> material;
        Resources::Load<Material>(&material, xml.Get<GUID>("Material"));
        SetMaterial(material.Get());
    }

    if (xml.Contains("RenderPass"))
    { SetRenderPass( xml.Get<RenderPass>("RenderPass") ); }

    if (xml.Contains("LineWidth"))
    { SetLineWidth(xml.Get<float>("LineWidth")); }

    if (xml.Contains("RenderWireframe"))
    { SetRenderWireframe(xml.Get<bool>("RenderWireframe")); }
}

void Renderer::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    Material* mat = GetMaterial();
    xmlInfo->Set("Visible", IsVisible());
    xmlInfo->Set("Material", mat ? mat->GetGUID() : GUID::Empty());
    xmlInfo->Set("RenderPass", GetRenderPass());
    xmlInfo->Set("LineWidth", GetLineWidth());
    xmlInfo->Set("RenderWireframe", IsRenderWireframe());
}
