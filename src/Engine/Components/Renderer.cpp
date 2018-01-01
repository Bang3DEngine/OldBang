#include "Bang/Renderer.h"

#include <functional>

#include "Bang/GL.h"
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
    SetMaterial(MaterialFactory::GetDefault().Get());
}

Renderer::~Renderer()
{
}

Material *Renderer::GetUserMaterial() const
{
    if (p_material) { return GetMaterial(); }
    return GetSharedMaterial();
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

    GL::SetCullFace( GetCullFace() ); // Culling states
    if (GetCulling()) { GL::Enable( GL_Test::CullFace ); }
    else { GL::Disable( GL_Test::CullFace ); }

    GL::LineWidth( GetLineWidth() );

    if (GetUserMaterial()) { GetUserMaterial()->Bind(); }
}

void Renderer::UnBind() const
{
    if (GetUserMaterial()) { GetUserMaterial()->UnBind(); }
}

void Renderer::SetVisible(bool visible)
{
    m_visible = visible;
}


void Renderer::SetMaterial(Material *mat)
{
    if (GetSharedMaterial() != mat)
    {
        p_sharedMaterial.Set(mat);
        p_material.Set(nullptr);
    }
}

void Renderer::SetRenderPass(RenderPass rp) { m_renderPass = rp; }

bool Renderer::IsVisible() const { return m_visible; }
Material* Renderer::GetSharedMaterial() const { return p_sharedMaterial.Get(); }
RenderPass Renderer::GetRenderPass() const { return m_renderPass; }
bool Renderer::IsRenderWireframe() const { return m_drawWireframe; }
AABox Renderer::GetAABBox() const { return AABox(); }
void Renderer::SetCullFace(GL_Face cullMode) { m_cullFace = cullMode; }
void Renderer::SetCulling(bool culling) { m_cullling = culling; }
GL_Face Renderer::GetCullFace() const { return m_cullFace; }
bool Renderer::GetCulling() const { return m_cullling; }
void Renderer::SetLineWidth(float w) { m_lineWidth = w; }
void Renderer::SetRenderWireframe(bool drawWireframe)
{
    m_drawWireframe = drawWireframe;
}
void Renderer::SetViewProjMode(GL_ViewProjMode viewProjMode)
{
    m_viewProjMode = viewProjMode;
}
void Renderer::SetRenderPrimitive(GL_Primitive renderMode)
{
    m_renderMode = renderMode;
}

GL_ViewProjMode Renderer::GetViewProjMode() const { return m_viewProjMode; }
GL_Primitive Renderer::GetRenderPrimitive() const { return m_renderMode; }
float Renderer::GetLineWidth() const { return m_lineWidth; }
Material* Renderer::GetMaterial() const
{
    if (!p_material)
    {
        if (GetSharedMaterial())
        {
            p_material = Resources::Clone<Material>(GetSharedMaterial());
        }
    }
    return p_material.Get();
}

Rect Renderer::GetBoundingRect(Camera *camera) const
{
    return Rect::NDCRect;
    return camera ? camera->GetScreenBoundingRect(GetAABBox()) :
                    Rect::Zero;
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

    xmlInfo->Set("Visible", IsVisible());

    Material* sMat = GetSharedMaterial();
    xmlInfo->Set("Material", sMat ? sMat->GetGUID() : GUID::Empty());

    xmlInfo->Set("RenderPass", GetRenderPass());
    xmlInfo->Set("LineWidth", GetLineWidth());
    xmlInfo->Set("RenderWireframe", IsRenderWireframe());
}
