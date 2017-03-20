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
    m_materialSP = AssetsManager::Load<Material>("Materials/UI/SP_UIImage.bmat",
                                                 true);
    SetTransparent(false);
    SetDepthLayer(Renderer::DepthLayer::DepthLayerCanvas);

    m_materialSP = new Material();
    m_material->SetDiffuseColor(Color::White);
}

UIRenderer::~UIRenderer()
{
}

void UIRenderer::Bind() const
{
    MeshRenderer::Bind();

    ShaderProgram *sp = GetMaterial()->GetShaderProgram();
    sp->SetColor("B_Tint", m_tint);
}

void UIRenderer::CloneInto(ICloneable *clone) const
{
    UIRenderer *rend = Object::SCast<UIRenderer>(clone);
    MeshRenderer::CloneInto(rend);
    rend->m_tint = m_tint;
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
    m_tint = xmlInfo.GetColor("Tint");
}

void UIRenderer::Write(XMLNode *xmlInfo) const
{
    MeshRenderer::Write(xmlInfo);

    xmlInfo->SetColor("Tint", m_tint);
}
