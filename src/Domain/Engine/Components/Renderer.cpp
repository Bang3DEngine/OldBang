#include "Renderer.h"

#include <functional>

#include "AABox.h"
#include "Debug.h"
#include "Scene.h"
#include "Camera.h"
#include "GBuffer.h"
#include "Material.h"
#include "Transform.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "AssetsManager.h"
#include "ShaderProgram.h"
#include "ShaderContract.h"
#include "GraphicPipeline.h"

#ifdef BANG_EDITOR
#include "EditorScene.h"
#include "SelectionFramebuffer.h"
#endif

Renderer::Renderer()
{
    #ifdef BANG_EDITOR
    p_OnRenderingStartsForSelectionFunc = [](){};
    #endif

    m_material = AssetsManager::Load<Material>("Materials/G_Default.bmat", true);
}

Renderer::~Renderer()
{

}

void Renderer::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Renderer *r = Object::SCast<Renderer>(clone);
    r->SetMaterial(GetMaterial());
    r->SetDrawWireframe(GetDrawWireframe());
    r->SetCullMode(GetCullMode());
    r->SetRenderMode(GetRenderMode());
    r->SetLineWidth(GetLineWidth());
    r->SetClosedInInspector(IsClosedInInspector());
    r->SetTransparent(IsTransparent());
}

Material *Renderer::GetMaterial() const
{
    return m_material ? m_material : Material::GetMissingMaterial();
}

void Renderer::OnRenderingStarts(GameObject *go, ShaderProgram *sp) const
{
    GL::SetWireframe(m_drawWireframe);
    GL::SetCullMode(m_cullMode);
    glLineWidth(m_lineWidth);

    go->OnRenderingStarts(go, sp);
    sp->OnRenderingStarts(go, sp);

    Scene *scene = SceneManager::GetActiveScene();
    Camera *camera = scene->GetCamera();
    if (camera) { camera->OnRenderingStarts(go, sp); }

    GBuffer *gb = GraphicPipeline::GetActive()->GetGBuffer();
    gb->OnRenderingStarts(go, sp);
}

void Renderer::RenderForSelectionWithoutMaterial() const
{
    RenderWithoutMaterial();
}

void Renderer::Render() const
{
    RenderWithMaterial(m_material);
}

void Renderer::RenderWithMaterial(Material *_mat) const
{
    Renderer *ncThis = const_cast<Renderer*>(this);
    Material *mat = _mat ? _mat : Material::GetMissingMaterial();

    ShaderProgram *sp = mat->GetShaderProgram();
    ncThis->OnRenderingStarts(gameObject, sp);
    mat->OnRenderingStarts(gameObject, sp);

    sp->OnJustBeforeRendering(gameObject, sp);
    mat->OnJustBeforeRendering(gameObject, sp);
    ncThis->OnJustBeforeRendering(gameObject, sp);

    GL::Apply();
    GL::ApplyToShaderProgram(sp);

    #ifdef BANG_EDITOR
    GraphicPipeline *gp = GraphicPipeline::GetActive();
    SelectionFramebuffer *sfb = gp->GetSelectionFramebuffer();
    if (sfb->IsPassing())
    {
        p_OnRenderingStartsForSelectionFunc();
        RenderForSelectionWithoutMaterial();
    }
    else
    #endif
    {
        RenderWithoutMaterial();
    }

    ncThis->OnJustAfterRendering(gameObject, sp);
    mat->OnJustAfterRendering(gameObject, sp);
    sp->OnJustAfterRendering(gameObject, sp);

    GL::Reset();

    ncThis->OnRenderingEnds(gameObject, sp);
    mat->OnRenderingEnds(gameObject, sp);
}

void Renderer::OnRenderingEnds(GameObject *go, ShaderProgram *sp) const
{
    sp->OnRenderingEnds(go, sp);
}

void Renderer::SetTransparent(bool transparent)
{
    m_isTransparent = transparent;
}
bool Renderer::IsTransparent() const
{
    return m_isTransparent;
}

void Renderer::SetDepthLayer(Renderer::DepthLayer dl)
{
    m_depthLayer = dl;
}

Renderer::DepthLayer Renderer::GetDepthLayer() const
{
    return m_depthLayer;
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

#ifdef BANG_EDITOR
void Renderer::SetOnRenderingStartsForSelectionFunction(
        const std::function<void()> &f)
{
    p_OnRenderingStartsForSelectionFunc = f;
}
#endif

String Renderer::GetName() const
{
    return "Renderer";
}

void Renderer::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);

    String materialFilepath = xmlInfo->GetFilepath("Material");
    if (!materialFilepath.Empty())
    {
        SetMaterial( AssetsManager::Load<Material>(materialFilepath) );
    }
    else
    {
        SetMaterial (nullptr);
    }

    SetTransparent(xmlInfo->GetBool("IsTransparent"));
    SetLineWidth(xmlInfo->GetFloat("LineWidth"));
    SetDrawWireframe(xmlInfo->GetBool("DrawWireframe"));
}

void Renderer::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Renderer");

    if (m_material)
    {
        if (m_material->GetFilepath() != "")
        {
            xmlInfo->SetFilepath("Material", m_material->GetFilepath(), "bmat");
        }
        else //In case the asset is created in runtime, write its mem address
        {
            String memAddress = String::ToString((void*) m_material);
            xmlInfo->SetFilepath("Material", memAddress, "bmat");
        }
    }
    else
    {
        xmlInfo->SetFilepath("Material", "", "bmat");
    }

    xmlInfo->SetFloat("LineWidth", GetLineWidth());
    xmlInfo->SetBool("IsTransparent", IsTransparent(), {XMLProperty::Inline});
    xmlInfo->SetBool("DrawWireframe", GetDrawWireframe(), {XMLProperty::Inline});
}
