#include "UIRenderer.h"

#include "Rect.h"
#include "Material.h"
#include "MeshFactory.h"
#include "AssetsManager.h"
#include "SelectionFramebuffer.h"

UIRenderer::UIRenderer()
{
    m_hasCustomPRPass = true;

    SetMesh(MeshFactory::GetPlane());
    SetMaterial( AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/D2G_Default.bmat") );
    m_materialPR = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/UI/PR_UIImage.bmat");

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
    MeshRenderer::CloneInto(clone);
}

ICloneable *UIRenderer::Clone() const
{
    return MeshRenderer::Clone();
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
    // Dont take into account View nor Projection, coords already
    // are in NDC
    Box screenBox = gameObject->GetBoundingBox();
    return Rect(screenBox.GetMin().xy(), screenBox.GetMax().xy());
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
    MeshRenderer::Render();
}

const String UIRenderer::ToString() const
{
    return "UIRenderer";
}

String UIRenderer::GetName() const
{
    return "UIRenderer";
}

#ifdef BANG_EDITOR
void UIRenderer::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void UIRenderer::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
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
#endif
