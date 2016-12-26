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
#include "SelectionFramebuffer.h"

UIImage::UIImage()
{
}

UIImage::~UIImage()
{
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
    Texture2D *tex = AssetsManager::LoadAsset<Texture2D>(texFilepath);
    if (m_material)
    {
        m_material->SetTexture(tex);
    }
}

void UIImage::FillXMLInfo(XMLNode *xmlInfo) const
{
    UIRenderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName(GetName());

    String texFilepath = "";
    if (m_material)
    {
        const Texture2D *tex = m_material->GetTexture();
        texFilepath = tex ? tex->GetFilepath() : "";
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

    ShaderProgram *sp = m_materialPR->GetShaderProgram();
    sp->SetUniformColor("B_tint",        m_tint);
    sp->SetUniformFloat("B_stroke",      m_stroke);
    sp->SetUniformColor("B_strokeColor", m_strokeColor);
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

