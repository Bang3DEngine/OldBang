#include "UIImage.h"

#include "Rect.h"
#include "Material.h"
#include "MeshFactory.h"
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
    MeshRenderer::CloneInto(clone);
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

