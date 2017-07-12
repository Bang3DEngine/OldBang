#include "Bang/Texture2D.h"

#include "Bang/XMLNode.h"

Texture2D::Texture2D() : G_Texture2D()
{
}

Texture2D::Texture2D(const Path &imageFilepath) : G_Texture2D(imageFilepath)
{
}

Texture2D::Texture2D(const G_Texture2D *gTexture) : G_Texture2D(*gTexture)
{
}

Texture2D::~Texture2D()
{
}

String Texture2D::GetFileExtensionStatic()
{
    return "btex2d";
}

String Texture2D::GetFileExtension() const
{
    return Texture2D::GetFileExtensionStatic();
}

void Texture2D::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);

    Path imageFilepath = xmlInfo.GetFilepath("ImageFilepath");
    LoadFromImage(imageFilepath);

    SetFilterMode( xmlInfo.GetEnum<FilterMode>("FilterMode") );
    SetAlphaCutoff( xmlInfo.GetFloat("AlphaCutoff") );
}

void Texture2D::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);

    xmlInfo->SetFilepath("ImageFilepath", m_imageFilepath);

    xmlInfo->SetEnum<FilterMode>("FilterMode", GetFilterMode());
    xmlInfo->SetFloat("AlphaCutoff", GetAlphaCutoff());
}
