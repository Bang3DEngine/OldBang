#include "UIImage.h"

#include "Material.h"
#include "MeshFactory.h"
#include "GraphicPipeline.h"
#include "SelectionFramebuffer.h"

UIImage::UIImage()
{
    SetMesh(MeshFactory::GetPlane());
    SetMaterial(AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/UI/UIImage.bmat") );
    m_materialPR = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/UI/PR_UIImage.bmat");

    SetIgnoreModelMatrix(false);
    SetIgnoreViewMatrix(true);
    SetIgnoreProjectionMatrix(true);
    SetReceivesLighting(false);
    SetTransparent(false);
    SetDepthLayer(Renderer::DepthLayer::DepthLayerCanvas);

    m_material->SetDiffuseColor(Color::white);
}

UIImage::~UIImage()
{
}

void UIImage::Render() const
{
    MeshRenderer::Render();

    #ifdef BANG_EDITOR
    SelectionFramebuffer *sfb = GraphicPipeline::GetActive()->GetSelectionFramebuffer();
    if (!sfb || !sfb->IsPassing())
    #endif
    {
        GBuffer *gbuffer = GraphicPipeline::GetActive()->GetGBuffer();
        gbuffer->SetColorDrawBuffer();
        gbuffer->BindInputTexturesTo(m_materialPR);
        m_materialPR->GetShaderProgram()->SetUniformColor("B_tint", m_tint);
        m_materialPR->GetShaderProgram()->SetUniformColor("B_strokeColor", m_strokeColor);
        m_materialPR->GetShaderProgram()->SetUniformFloat("B_stroke", m_stroke);
        RenderWithMaterial(m_materialPR);
    }
}

bool UIImage::IsACanvasRenderer() const
{
    return true;
}

const String UIImage::ToString() const
{
    return "UIImage";
}

String UIImage::GetName() const
{
    return "UIImage";
}

void UIImage::CloneInto(ICloneable *clone) const
{
    MeshRenderer::CloneInto(clone);
    UIImage *img = static_cast<UIImage*>(clone);
}

ICloneable *UIImage::Clone() const
{
    UIImage *img = new UIImage();
    CloneInto(img);
    return img;
}

#ifdef BANG_EDITOR
void UIImage::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void UIImage::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}
#endif

void UIImage::ReadXMLInfo(const XMLNode *xmlInfo)
{
    MeshRenderer::ReadXMLInfo(xmlInfo);

    String texFilepath = xmlInfo->GetFilepath("Image");
    Texture2D *tex = AssetsManager::LoadAsset<Texture2D>(texFilepath);
    m_material->SetTexture(tex);

    m_tint = xmlInfo->GetColor("Tint");
    m_strokeColor = xmlInfo->GetColor("StrokeColor");
    m_stroke = xmlInfo->GetFloat("Stroke");
}

void UIImage::FillXMLInfo(XMLNode *xmlInfo) const
{
    MeshRenderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName(GetName());

    const Texture2D *tex = m_material->GetTexture();
    String texFilepath = tex ? tex->GetFilepath() : "";
    xmlInfo->SetFilepath("Image", texFilepath,
                         Texture2D::GetFileExtensionStatic(), {});
    xmlInfo->SetColor("Tint", m_tint);
    xmlInfo->SetColor("StrokeColor", m_strokeColor);
    xmlInfo->SetFloat("Stroke", m_stroke);

    xmlInfo->GetAttribute("Mesh")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("Material")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("LineWidth")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("IsTransparent")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("DrawWireframe")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("ReceivesLighting")->SetProperty({XMLProperty::Hidden});
}

