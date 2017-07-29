#include "Bang/Renderer.h"

#include <functional>

#include "Bang/Rect.h"
#include "Bang/AABox.h"
#include "Bang/Camera.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/AssetsManager.h"
#include "Bang/ShaderProgram.h"

Renderer::Renderer()
{
    SetMaterial(
           AssetsManager::Load<Material>( EPATH("Materials/G_Default.bmat") ));
}

Renderer::~Renderer()
{
    if (m_materialCopy)
    {
        delete m_materialCopy;
    }
}

void Renderer::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Renderer *r = SCAST<Renderer*>(clone);
    r->SetMaterial(GetSharedMaterial());
    r->SetDrawWireframe(GetDrawWireframe());
    r->SetCullMode(GetCullMode());
    r->SetRenderMode(GetRenderMode());
    r->SetLineWidth(GetLineWidth());
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

Material *Renderer::GetMaterial() const
{
    if (m_materialCopy) { return m_materialCopy; }
    return m_material ? m_material : Material::GetMissingMaterial();
}

Material *Renderer::GetSharedMaterial() const
{
    return m_material;
}

void Renderer::Bind() const
{
    GL::SetViewProjMode(m_viewProjMode);
    GL::SetWireframe(m_drawWireframe);
    GL::SetCullMode(m_cullMode);
    glLineWidth(m_lineWidth);

    Matrix4 model; transform->GetLocalToWorldMatrix(&model);
    GL::SetModelMatrix(model);

    Material *mat = GetMaterial();
    mat->Bind();
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

void Renderer::UnBind() const
{
    GetMaterial()->UnBind();
}

void Renderer::SetRenderLayer(Renderer::RenderLayer rl)
{
    m_renderLayer = rl;
}

Renderer::RenderLayer Renderer::GetRenderLayer() const
{
    return m_renderLayer;
}

void Renderer::SetDrawWireframe(bool drawWireframe)
{
    m_drawWireframe = drawWireframe;
}
bool Renderer::GetDrawWireframe() const
{
    return m_drawWireframe;
}

Rect Renderer::GetBoundingRect(Camera *camera) const
{
    return camera->GetScreenBoundingRect(GetAABBox());
}


void Renderer::SetCullMode(GL::CullMode cullMode)
{
    m_cullMode = cullMode;
}
GL::CullMode Renderer::GetCullMode() const
{
    return m_cullMode;
}

void Renderer::SetViewProjMode(GL::ViewProjMode viewProjMode)
{
    m_viewProjMode = viewProjMode;
}

GL::ViewProjMode Renderer::GetViewProjMode() const
{
    return m_viewProjMode;
}


void Renderer::SetRenderMode(GL::RenderMode renderMode)
{
    m_renderMode = renderMode;
}
GL::RenderMode Renderer::GetRenderMode() const
{
    return m_renderMode;
}


void Renderer::SetLineWidth(float w)
{
    m_lineWidth = w;
}
float Renderer::GetLineWidth() const
{
    return m_lineWidth;
}

void Renderer::Read(const XMLNode &xmlInfo)
{
    Component::Read(xmlInfo);

    Path materialFilepath = xmlInfo.Get<Path>("Material");
    SetMaterial( AssetsManager::Load<Material>(materialFilepath) );
    SetLineWidth(xmlInfo.Get<float>("LineWidth"));
    SetDrawWireframe(xmlInfo.Get<bool>("DrawWireframe"));
}

void Renderer::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);

    Material *sharedMat = GetSharedMaterial();
    xmlInfo->Set("Material", sharedMat ? sharedMat->GetFilepath() :
                                                 Path::Empty);
    xmlInfo->Set("LineWidth", GetLineWidth());
    xmlInfo->Set("DrawWireframe", GetDrawWireframe());
}
