#include "UIRenderer.h"

#include "Mesh.h"
#include "Rect.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshFactory.h"
#include "ShaderProgram.h"
#include "AssetsManager.h"

UIRenderer::UIRenderer()
{
    m_hasCustomSPPass = true;

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

void UIRenderer::OnJustBeforeRendering(Material *mat) const
{
    MeshRenderer::OnJustBeforeRendering(mat);
    mat->GetShaderProgram()->SetUniformColor("B_material_diffuse_color",
                                             m_tint);
}

void UIRenderer::CloneInto(ICloneable *clone) const
{
    UIRenderer *rend = Object::SCast<UIRenderer>(clone);
    MeshRenderer::CloneInto(rend);
    rend->m_tint = m_tint;
}

ICloneable *UIRenderer::Clone() const
{
    UIRenderer *uiRend = new UIRenderer();
    CloneInto(uiRend);
    return uiRend;
}

void UIRenderer::SetMatricesUniforms(Material *mat,
                                     const Matrix4 &model,
                                     const Matrix4 &normal,
                                     const Matrix4 &view,
                                     const Matrix4 &projection,
                                     const Matrix4 &pvm) const
{
    MeshRenderer::SetMatricesUniforms(mat,
                                      model,
                                      normal,
                                      Matrix4::Identity,
                                      Matrix4::Identity,
                                      model);
}

Rect UIRenderer::GetBoundingRect(Camera *camera) const
{
    return MeshRenderer::GetBoundingRect(camera);
}

void UIRenderer::RenderCustomSP() const
{
    MeshRenderer::RenderCustomSP();
}

bool UIRenderer::IsACanvasRenderer() const
{
    return true;
}

void UIRenderer::RenderWithoutBindingMaterial() const
{
    glDisable(GL_DEPTH_TEST);
    MeshRenderer::RenderWithoutBindingMaterial();
    glEnable(GL_DEPTH_TEST);
}

String UIRenderer::GetName() const
{
    return "UIRenderer";
}

void UIRenderer::ReadXMLInfo(const XMLNode *xmlInfo)
{
    MeshRenderer::ReadXMLInfo(xmlInfo);

    m_tint = xmlInfo->GetColor("Tint");
}

void UIRenderer::FillXMLInfo(XMLNode *xmlInfo) const
{
    MeshRenderer::FillXMLInfo(xmlInfo);

    xmlInfo->SetColor("Tint", m_tint);
}
