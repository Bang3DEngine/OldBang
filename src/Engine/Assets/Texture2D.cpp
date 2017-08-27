#include "Bang/Texture2D.h"

#include "Bang/XMLNode.h"
#include "Bang/Resources.h"

Texture2D::Texture2D() : G_Texture2D()
{
}

Texture2D::Texture2D(const Path &imageFilepath)
{
    Import(imageFilepath);
}

Texture2D::Texture2D(const G_Texture2D *gTexture) : G_Texture2D(*gTexture)
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);

    if (xmlInfo.Contains("FilterMode"))
    { SetFilterMode( xmlInfo.Get<FilterMode>("FilterMode") ); }

    if (xmlInfo.Contains("AlphaCuttoff"))
    { SetAlphaCutoff( xmlInfo.Get<float>("AlphaCutoff") ); }
}

void Texture2D::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);

    xmlInfo->Set("FilterMode", GetFilterMode());
    xmlInfo->Set("AlphaCutoff", GetAlphaCutoff());
}

void Texture2D::Import(const Path &imageFilepath)
{
    G_Image *img = Resources::Load<G_Image>(imageFilepath);
    if (img) { LoadFromImage(*img); }

    Path importFilepath = ImportFilesManager::GetImportFilePath(imageFilepath);
    ReadFromFile(importFilepath);
}
