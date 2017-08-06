#include "Bang/UIImageRenderer.h"

#include "Bang/Material.h"
#include "Bang/Texture2D.h"
#include "Bang/AssetsManager.h"

UIImageRenderer::UIImageRenderer()
{
    SetMaterial(AssetsManager::Load<Material>(
                    EPATH("Materials/UI/G_UIImageRenderer.bmat") ) );
    UseMaterialCopy();

    m_imageTexture = new Texture2D();
    m_imageTexture->CreateEmpty(1,1);
}

UIImageRenderer::~UIImageRenderer()
{
}

void UIImageRenderer::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);
    UIImageRenderer *img = SCAST<UIImageRenderer*>(clone);
    img->SetImage( GetImageTexture() );
}

void UIImageRenderer::SetImage(Texture2D *imageTexture)
{
    m_imageTexture = imageTexture;
    GetMaterial()->SetTexture(m_imageTexture);
}

Texture2D *UIImageRenderer::GetImageTexture() const
{
    return m_imageTexture;
}

void UIImageRenderer::Read(const XMLNode &xmlInfo)
{
    UIRenderer::Read(xmlInfo);
    Path texFilepath = xmlInfo.Get<Path>("Image");
    SetImage( AssetsManager::Load<Texture2D>(texFilepath) );
}

void UIImageRenderer::Write(XMLNode *xmlInfo) const
{
    UIRenderer::Write(xmlInfo);

    Texture2D *imgTex = GetImageTexture();
    Path texFilepath = imgTex ? imgTex->GetFilepath() : Path();
    xmlInfo->Set("Image", texFilepath);
}

