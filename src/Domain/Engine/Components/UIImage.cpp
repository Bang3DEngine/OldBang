#include "UIImage.h"

#include "Rect.h"
#include "GBuffer.h"
#include "Material.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "MeshFactory.h"
#include "ShaderProgram.h"
#include "AssetsManager.h"
#include "GraphicPipeline.h"

UIImage::UIImage()
{
    m_material = AssetsManager::Load<Material>("Materials/UI/D2G_UIImage.bmat", true);
    m_materialPR = AssetsManager::Load<Material>("Materials/UI/PR_UIImage.bmat", true);
}

UIImage::~UIImage()
{
}

String UIImage::GetName() const
{
    return "UIImage";
}

void UIImage::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);
    UIImage *img = static_cast<UIImage*>(clone);
}

ICloneable *UIImage::Clone() const
{
    UIImage *img = new UIImage();
    CloneInto(img);
    return img;
}

void UIImage::ReadXMLInfo(const XMLNode *xmlInfo)
{
    UIRenderer::ReadXMLInfo(xmlInfo);

    String texFilepath = xmlInfo->GetFilepath("Image");
    m_imageTexture = AssetsManager::Load<Texture2D>(texFilepath);
}

void UIImage::FillXMLInfo(XMLNode *xmlInfo) const
{
    UIRenderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName(GetName());

    String texFilepath = "";
    if (m_imageTexture)
    {
        String texFilepath = m_imageTexture ? m_imageTexture->GetFilepath() : "";
        xmlInfo->SetFilepath("Image", texFilepath,
                             Texture2D::GetFileExtensionStatic(), {});
    }

    xmlInfo->GetAttribute("Mesh")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("Material")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("LineWidth")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("IsTransparent")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("DrawWireframe")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("ReceivesLighting")->SetProperty({XMLProperty::Hidden});
}

void UIImage::RenderCustomPR() const
{
    UIRenderer::RenderCustomPR();

    ASSERT(m_material); ASSERT(m_materialPR);

    ShaderProgram *sp = m_materialPR->GetShaderProgram();
    sp->SetUniformColor("B_tint",        m_tint);
    sp->SetUniformTexture("B_texture_0", m_imageTexture);

    AABox screenBox = gameObject->GetAABBox();
    Rect renderRect(screenBox.GetMin().xy(), screenBox.GetMax().xy());
    GBuffer *gb = GraphicPipeline::GetActive()->GetGBuffer();
    gb->RenderPassWithMaterial(m_materialPR, renderRect);
}

void UIImage::RenderWithMaterial(Material *mat) const
{
    if (mat == m_material)
    {
        m_material->SetTexture(m_imageTexture);
    }
    UIRenderer::RenderWithMaterial(mat);
}

