#include "Bang/Renderer.h"

#include <functional>

#include "Bang/Debug.h"
#include "Bang/Camera.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
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
    Renderer *r = Object::SCast<Renderer>(clone);
    r->SetMaterial(GetSharedMaterial());
    r->SetDrawWireframe(GetDrawWireframe());
    r->SetCullMode(GetCullMode());
    r->SetRenderMode(GetRenderMode());
    r->SetLineWidth(GetLineWidth());
    r->SetClosedInInspector(IsClosedInInspector());

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
    GL::SetWireframe(m_drawWireframe);
    GL::SetCullMode(m_cullMode);
    glLineWidth(m_lineWidth);

    Matrix4 model; transform->GetLocalToWorldMatrix(&model);
    GL::SetModelMatrix(model);

    Material *mat = GetMaterial();
    mat->Bind();
    G_ShaderProgram *sp = mat->GetShaderProgram();
    sp->SetBool("B_IsSelected", gameObject->IsSelected());
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

    Path materialFilepath = xmlInfo.GetFilepath("Material");
    SetMaterial( AssetsManager::Load<Material>(materialFilepath) );
    SetLineWidth(xmlInfo.GetFloat("LineWidth"));
    SetDrawWireframe(xmlInfo.GetBool("DrawWireframe"));
}

void Renderer::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);

    Material *sharedMat = GetSharedMaterial();
    xmlInfo->SetFilepath("Material", sharedMat ? sharedMat->GetFilepath() :
                                                 Path::Empty, "bmat");
    xmlInfo->SetFloat("LineWidth", GetLineWidth());
    xmlInfo->SetBool("DrawWireframe", GetDrawWireframe(), {XMLProperty::Inline});
}
