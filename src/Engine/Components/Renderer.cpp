#include "Bang/Renderer.h"

#include <functional>

#include "Bang/Rect.h"
#include "Bang/AABox.h"
#include "Bang/Camera.h"
#include "Bang/Transform.h"
#include "Bang/G_GBuffer.h"
#include "Bang/Resources.h"
#include "Bang/ShaderProgram.h"
#include "Bang/MaterialFactory.h"
#include "Bang/GraphicPipeline.h"

Renderer::Renderer()
{
    SetMaterial( MaterialFactory::GetDefault() );
}

Renderer::~Renderer()
{
    if (m_materialCopy) { delete m_materialCopy; }
}

void Renderer::OnRender(RenderPass renderPass)
{
    Component::OnRender(renderPass);
    if (GetVisible() && renderPass == GetRenderPass())
    {
        GraphicPipeline::GetActive()->Render(this);
    }
}
void Renderer::OnRender() { }

void Renderer::Bind() const
{
    GL::SetViewProjMode(m_viewProjMode);
    GL::SetWireframe(m_drawWireframe);
    GL::SetCullMode(m_cullMode);
    glLineWidth(m_lineWidth);

    Matrix4 model;
    if (transform) { transform->GetLocalToWorldMatrix(&model); }
    GL::SetModelMatrix(model);

    Material *mat = GetMaterial();
    mat->Bind();
}

void Renderer::UnBind() const
{
    GetMaterial()->UnBind();
}

void Renderer::UseMaterialCopy()
{
    if (m_materialCopy)
    {
        delete m_materialCopy;
        m_materialCopy = nullptr;
    }

    if (GetSharedMaterial())
    {
        m_materialCopy = new Material();
        GetSharedMaterial()->CloneInto(m_materialCopy);
    }
}

void Renderer::SetVisible(bool visible)
{
    m_visible = visible;
}


void Renderer::SetMaterial(Material *m)
{
    if (m_material != m)
    {
        m_material = m;
        if (m_materialCopy)
        {
            delete m_materialCopy;
            m_materialCopy = nullptr;
        }
    }
}
void Renderer::SetRenderPass(RenderPass rp) { m_renderPass = rp; }

bool Renderer::GetVisible() const { return m_visible; }
RenderPass Renderer::GetRenderPass() const { return m_renderPass; }
bool Renderer::GetRenderWireframe() const { return m_drawWireframe; }
AABox Renderer::GetAABBox() const { return AABox(); }
void Renderer::SetCullMode(GL::CullMode cullMode) { m_cullMode = cullMode; }
GL::CullMode Renderer::GetCullMode() const { return m_cullMode; }
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
Material *Renderer::GetSharedMaterial() const { return m_material; }
Material *Renderer::GetMaterial() const
{
    if (m_materialCopy) { return m_materialCopy; }
    return m_material ? m_material : MaterialFactory::GetMissing();
}
Rect Renderer::GetBoundingRect(Camera *camera) const
{
    return camera->GetScreenBoundingRect(GetAABBox());
}

void Renderer::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Renderer *r = SCAST<Renderer*>(clone);
    r->SetMaterial(GetSharedMaterial());
    r->SetRenderWireframe(GetRenderWireframe());
    r->SetCullMode(GetCullMode());
    r->SetRenderPrimitive(GetRenderPrimitive());
    r->SetLineWidth(GetLineWidth());
}

void Renderer::Read(const XMLNode &xml)
{
    Component::Read(xml);

    if (xml.Contains("Material"))
    { SetMaterial( Resources::Load<Material>( xml.Get<GUID>("Material") ) ); }

    if (xml.Contains("RenderPass"))
    { SetRenderPass( xml.Get<RenderPass>("RenderPass") ); }

    if (xml.Contains("LineWidth"))
    { SetLineWidth(xml.Get<float>("LineWidth")); }

    if (xml.Contains("RenderWireframe"))
    { SetRenderWireframe(xml.Get<bool>("RenderWireframe")); }
}

void Renderer::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);

    Material *sharedMat = GetSharedMaterial();
    Debug_Log("MaterialFactory::GetDefaultUnLighted()->GetGUID(): " << MaterialFactory::GetDefaultUnLighted()->GetGUID());
    if (sharedMat) {
        Debug_Log("sharedMat->GetGUID(): " << sharedMat->GetGUID());
        GUID matGUID = sharedMat->GetGUID();
        Debug_Log("Saving " << GetClassName() << " with materialGUID: " << sharedMat->GetGUID());
    }
    xmlInfo->Set("Material", sharedMat ? sharedMat->GetGUID() : GUID::Empty());
    xmlInfo->Set("RenderPass", GetRenderPass());
    xmlInfo->Set("LineWidth", GetLineWidth());
    xmlInfo->Set("RenderWireframe", GetRenderWireframe());
}
