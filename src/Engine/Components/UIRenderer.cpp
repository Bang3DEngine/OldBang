#include "Bang/UIRenderer.h"

#include "Bang/GL.h"
#include "Bang/Mesh.h"
#include "Bang/Rect.h"
#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/MeshFactory.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"

UIRenderer::UIRenderer()
{
    SetMesh(MeshFactory::GetUIPlane());
    SetMaterial(
        AssetsManager::Load<Material>( EPATH("Materials/G_DefaultNoSP.bmat")) );
    UseMaterialCopy();
    GetMaterial()->SetDiffuseColor(m_tint);

    SetRenderLayer(Renderer::RenderLayer::Canvas);
}

UIRenderer::~UIRenderer()
{
}

void UIRenderer::SetTint(const Color &tint)
{
    m_tint = tint;
    GetMaterial()->SetDiffuseColor(m_tint);
}

const Color &UIRenderer::GetTint() const
{
    return m_tint;
}

void UIRenderer::Bind() const
{
    GL::SetViewProjMode( GL::ViewProjMode::IgnoreBoth );
    Renderer::Bind();
}

void UIRenderer::UnBind() const
{
    GL::SetViewProjMode( GL::ViewProjMode::UseBoth );
    MeshRenderer::UnBind();
}

void UIRenderer::CloneInto(ICloneable *clone) const
{
    UIRenderer *rend = SCAST<UIRenderer*>(clone);
    MeshRenderer::CloneInto(rend);
    rend->SetTint( GetTint() );
}

Rect UIRenderer::GetBoundingRect(Camera *camera) const
{
    return MeshRenderer::GetBoundingRect(camera);
}

void UIRenderer::Render() const
{
    GL::SetTestDepth(false);
    MeshRenderer::Render();
    GL::SetTestDepth(true);
}

void UIRenderer::Read(const XMLNode &xmlInfo)
{
    Renderer::Read(xmlInfo);
    SetTint( xmlInfo.GetColor("Tint") );
}

void UIRenderer::Write(XMLNode *xmlInfo) const
{
    Renderer::Write(xmlInfo);
    xmlInfo->SetColor("Tint", GetTint());
}
