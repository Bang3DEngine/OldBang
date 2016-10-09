#include "UIRenderer.h"

#include "Rect.h"
#include "Material.h"
#include "MeshFactory.h"
#include "GraphicPipeline.h"
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

    m_material->SetDiffuseColor(Color::White);
}

UIRenderer::~UIRenderer()
{
}

void UIRenderer::SetMatricesUniforms(Material *mat,
                                  const Matrix4 &model,
                                  const Matrix4 &normal,
                                  const Matrix4 &view,
                                  const Matrix4 &projection,
                                  const Matrix4 &pvm) const
{
    NONULL(mat); NONULL(mat->GetShaderProgram());

    MeshRenderer::SetMatricesUniforms(mat, model, normal, view,
                                      projection, pvm);

    ShaderProgram *sp = mat->GetShaderProgram();
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_Projection,
                       Matrix4::Identity);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_Projection_Inverse,
                       Matrix4::Identity);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_View,
                       Matrix4::Identity);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_View_Inverse,
                       Matrix4::Identity);
    sp->SetUniformMat4(ShaderContract::Uniform_Matrix_PVM, model);
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

    ShaderProgram *sp = m_materialPR->GetShaderProgram();
    sp->SetUniformColor("B_tint",        m_tint);
    sp->SetUniformColor("B_strokeColor", m_strokeColor);
    sp->SetUniformFloat("B_stroke",      m_stroke);
    sp->SetUniformTexture("B_texture_0", m_material->GetTexture());

    Box screenBox = gameObject->GetBoundingBox();
    sp->SetUniformFloat("B_image_left",  screenBox.GetMin().x);
    sp->SetUniformFloat("B_image_up",    screenBox.GetMax().y);
    sp->SetUniformFloat("B_image_right", screenBox.GetMax().x);
    sp->SetUniformFloat("B_image_bot",   screenBox.GetMin().y);

    Rect renderRect(screenBox.GetMin().xy(), screenBox.GetMax().xy());
    GBuffer *gb = GraphicPipeline::GetActive()->GetGBuffer();
    gb->RenderPassWithMaterial(m_materialPR, renderRect);
}

bool UIRenderer::IsACanvasRenderer() const
{
    return true;
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
#endif
