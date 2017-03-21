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
    m_materialCopy = new Material( *AssetsManager::Load<Material>(
                                        "Materials/UI/G_UIImage.bmat", true) );
    SetMaterial(m_materialCopy);

    m_materialSP = AssetsManager::Load<Material>("Materials/UI/SP_UIImage.bmat",
                                                 true);

    m_imageTexture = new Texture2D();
    m_imageTexture->CreateEmpty(1,1);
}

UIImage::~UIImage()
{
    delete m_materialCopy;
}

void UIImage::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);
    UIImage *img = Object::SCast<UIImage>(clone);
    img->SetImage( GetImage() );
}

void UIImage::SetImage(Texture2D *imageTexture)
{
    m_imageTexture = imageTexture;
    m_material->SetTexture(m_imageTexture);
}

Texture2D *UIImage::GetImage() const
{
    return m_imageTexture;
}

void UIImage::Read(const XMLNode &xmlInfo)
{
    UIRenderer::Read(xmlInfo);
    String texFilepath = xmlInfo.GetFilepath("Image");
    SetImage( AssetsManager::Load<Texture2D>(texFilepath) );
}

void UIImage::Write(XMLNode *xmlInfo) const
{
    UIRenderer::Write(xmlInfo);

    String texFilepath = m_imageTexture ? m_imageTexture->GetFilepath() : "";
    xmlInfo->SetFilepath("Image", texFilepath,
                         Texture2D::GetFileExtensionStatic());

    xmlInfo->GetAttribute("Mesh")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("Material")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("LineWidth")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("IsTransparent")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("DrawWireframe")->SetProperty({XMLProperty::Hidden});
}

