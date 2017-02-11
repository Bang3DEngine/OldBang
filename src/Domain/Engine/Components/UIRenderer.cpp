#include "UIRenderer.h"

#include "Mesh.h"
#include "Rect.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshFactory.h"
#include "AssetsManager.h"

UIRenderer::UIRenderer()
{
    m_hasCustomPRPass = true;

    SetMesh(MeshFactory::GetUIPlane());
    SetMaterial( AssetsManager::Load<Material>("Materials/D2G_Default.bmat", true) );
    m_materialPR = AssetsManager::Load<Material>("Materials/UI/PR_UIImage.bmat", true);

    SetReceivesLighting(false);
    SetTransparent(false);
    SetDepthLayer(Renderer::DepthLayer::DepthLayerCanvas);

    m_materialPR = new Material();
    m_material->SetDiffuseColor(Color::White);
}

UIRenderer::~UIRenderer()
{
}

void UIRenderer::CloneInto(ICloneable *clone) const
{
    UIRenderer *rend = static_cast<UIRenderer*>(clone);
    MeshRenderer::CloneInto(rend);
    rend->m_tint = m_tint;
    rend->m_stroke = m_stroke;
    rend->m_strokeColor = m_strokeColor;
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

void UIRenderer::RenderCustomPR() const
{
    MeshRenderer::RenderCustomPR();
}

bool UIRenderer::IsACanvasRenderer() const
{
    return true;
}

void UIRenderer::Render() const
{
    glDisable(GL_DEPTH_TEST);
    MeshRenderer::Render();
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
    m_strokeColor = xmlInfo->GetColor("StrokeColor");
    m_stroke = xmlInfo->GetFloat("Stroke");
}

void UIRenderer::FillXMLInfo(XMLNode *xmlInfo) const
{
    MeshRenderer::FillXMLInfo(xmlInfo);

    xmlInfo->SetColor("Tint", m_tint);
    xmlInfo->SetColor("StrokeColor", m_strokeColor);
    xmlInfo->SetFloat("Stroke", m_stroke);
}
