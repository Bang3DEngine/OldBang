#include "UIImage.h"

#include "Rect.h"
#include "Material.h"
#include "MeshFactory.h"
#include "GraphicPipeline.h"
#include "SelectionFramebuffer.h"

UIImage::UIImage()
{
    m_hasCustomPRPass = true;

    SetMesh(MeshFactory::GetPlane());
    SetMaterial( AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/D2G_Default.bmat") );
    m_materialPR = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/UI/PR_UIImage.bmat");

    SetIgnoreModelMatrix(false);
    SetIgnoreViewMatrix(true);
    SetIgnoreProjectionMatrix(true);
    SetReceivesLighting(false);
    SetTransparent(false);
    SetDepthLayer(Renderer::DepthLayer::DepthLayerCanvas);

    m_material->SetDiffuseColor(Color::White);
}

UIImage::~UIImage()
{
}

void UIImage::RenderCustomPR() const
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
    if (m_material)
    {
        m_material->SetTexture(tex);
    }

    m_tint = xmlInfo->GetColor("Tint");
    m_strokeColor = xmlInfo->GetColor("StrokeColor");
    m_stroke = xmlInfo->GetFloat("Stroke");
}

void UIImage::FillXMLInfo(XMLNode *xmlInfo) const
{
    MeshRenderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName(GetName());

    String texFilepath = "";
    if (m_material)
    {
        const Texture2D *tex = m_material->GetTexture();
        texFilepath = tex ? tex->GetFilepath() : "";
        xmlInfo->SetFilepath("Image", texFilepath,
                             Texture2D::GetFileExtensionStatic(), {});
    }

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

