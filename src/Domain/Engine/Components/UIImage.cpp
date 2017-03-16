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
    SetMaterial( AssetsManager::Load<Material>("Materials/UI/G_UIImage.bmat",
                                               true) );
    m_materialSP = AssetsManager::Load<Material>("Materials/UI/SP_UIImage.bmat",
                                                 true);

    m_imageTexture = new Texture2D();
    m_imageTexture->CreateEmpty(1,1);
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
    UIImage *img = Object::SCast<UIImage>(clone);
    img->SetTexture( GetTexture() );
}
ICloneable *UIImage::CloneVirtual() const { return _Clone<UIImage>(); }

void UIImage::OnJustBeforeRendering(GameObject *go,
                                    ShaderProgram *sp)
{
    UIRenderer::OnJustBeforeRendering(go, sp);
    sp->SetFloat("B_AlphaCutoff", !m_imageTexture ?
                                  1 : m_imageTexture->GetAlphaCutoff());
    sp->SetFloat("B_HasTexture", m_imageTexture ? 1 : 0);
    sp->SetTexture("B_Texture0", m_imageTexture);
}

void UIImage::SetTexture(Texture2D *imageTexture)
{
    m_imageTexture = imageTexture;
}

Texture2D *UIImage::GetTexture() const
{
    return m_imageTexture;
}

void UIImage::Read(const XMLNode &xmlInfo)
{
    UIRenderer::Read(xmlInfo);
    String texFilepath = xmlInfo.GetFilepath("Image");
    m_imageTexture = AssetsManager::Load<Texture2D>(texFilepath);
}

void UIImage::Write(XMLNode *xmlInfo) const
{
    UIRenderer::Write(xmlInfo);
    xmlInfo->SetTagName(GetName());

    String texFilepath = m_imageTexture ? m_imageTexture->GetFilepath() : "";
    xmlInfo->SetFilepath("Image", texFilepath,
                         Texture2D::GetFileExtensionStatic(), {});

    xmlInfo->GetAttribute("Mesh")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("Material")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("LineWidth")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("IsTransparent")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("DrawWireframe")->SetProperty({XMLProperty::Hidden});
}

