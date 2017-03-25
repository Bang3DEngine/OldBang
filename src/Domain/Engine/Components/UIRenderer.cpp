#include "UIRenderer.h"

#include "GL.h"
#include "Mesh.h"
#include "Rect.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshFactory.h"
#include "ShaderProgram.h"
#include "AssetsManager.h"

UIRenderer::UIRenderer()
{
    SetMesh(MeshFactory::GetUIPlane());
    SetMaterial( AssetsManager::Load<Material>("Materials/G_DefaultNoSP.bmat",
                                               true) );
    UseMaterialCopy();
    GetMaterial()->SetDiffuseColor(m_tint);

    SetTransparent(false);
    SetDepthLayer(Renderer::DepthLayer::DepthLayerCanvas);
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
    MeshRenderer::Bind();
}

void UIRenderer::UnBind() const
{
    GL::SetViewProjMode( GL::ViewProjMode::UseBoth );
    MeshRenderer::UnBind();
}

void UIRenderer::CloneInto(ICloneable *clone) const
{
    UIRenderer *rend = Object::SCast<UIRenderer>(clone);
    MeshRenderer::CloneInto(rend);
    rend->SetTint( GetTint() );
}

Rect UIRenderer::GetBoundingRect(Camera *camera) const
{
    return MeshRenderer::GetBoundingRect(camera);
}

void UIRenderer::RenderWithoutMaterial() const
{
    GL::SetTestDepth(false);
    MeshRenderer::RenderWithoutMaterial();
    GL::SetTestDepth(true);
}

void UIRenderer::Read(const XMLNode &xmlInfo)
{
    MeshRenderer::Read(xmlInfo);
    SetTint( xmlInfo.GetColor("Tint") );
}

void UIRenderer::Write(XMLNode *xmlInfo) const
{
    MeshRenderer::Write(xmlInfo);
    xmlInfo->SetColor("Tint", GetTint());
}
