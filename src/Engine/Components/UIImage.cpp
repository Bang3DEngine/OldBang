#include "Bang/UIImage.h"

#include "Bang/Rect.h"
#include "Bang/G_GBuffer.h"
#include "Bang/Material.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/MeshFactory.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"
#include "Bang/GraphicPipeline.h"

UIImage::UIImage()
{
    SetMaterial(AssetsManager::Load<Material>(
                    EPATH("Materials/UI/G_UIImage.bmat") ) );
    UseMaterialCopy();

    m_imageTexture = new Texture2D();
    m_imageTexture->CreateEmpty(1,1);
}

UIImage::~UIImage()
{
}

void UIImage::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);
    UIImage *img = Object::SCast<UIImage>(clone);
    img->SetImage( GetImageTexture() );
}

void UIImage::SetImage(Texture2D *imageTexture)
{
    m_imageTexture = imageTexture;
    GetMaterial()->SetTexture(m_imageTexture);
}

Texture2D *UIImage::GetImageTexture() const
{
    return m_imageTexture;
}

void UIImage::Read(const XMLNode &xmlInfo)
{
    UIRenderer::Read(xmlInfo);
    Path texFilepath = xmlInfo.GetFilepath("Image");
    SetImage( AssetsManager::Load<Texture2D>(texFilepath) );
}

void UIImage::Write(XMLNode *xmlInfo) const
{
    UIRenderer::Write(xmlInfo);

    Texture2D *imgTex = GetImageTexture();
    Path texFilepath = imgTex ? imgTex->GetFilepath() : Path();
    xmlInfo->SetFilepath("Image", texFilepath,
                         Texture2D::GetFileExtensionStatic());
    xmlInfo->GetAttribute("Mesh")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("Material")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("LineWidth")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("DrawWireframe")->SetProperty({XMLProperty::Hidden});
}

