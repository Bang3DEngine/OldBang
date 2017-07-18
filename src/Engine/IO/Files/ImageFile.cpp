#include "Bang/ImageFile.h"

#include "Bang/Debug.h"
#include "Bang/G_Image.h"

ImageFile::ImageFile()
{
}

ImageFile::ImageFile(const Path& path)
    : BFile(path)
{
    G_Image img = G_Image::FromFile(GetPath());
    m_width  = img.GetWidth();
    m_height = img.GetHeight();
    m_numComponents = 4;
}

void ImageFile::Read(const XMLNode &xmlInfo)
{
    BFile::Read(xmlInfo);
}

void ImageFile::Write(XMLNode *xmlInfo) const
{
    BFile::Write(xmlInfo);

    xmlInfo->SetString("FileName", GetPath().GetNameExt());
    xmlInfo->SetFilepath("ImageFilePath", GetPath());
    xmlInfo->SetString("Dimensions",
                       String(GetWidth()) + "x" + String(GetHeight()));
    xmlInfo->SetString("Mode", GetNumComponents() == 3 ? "RGB" : "RGBA");
}

bool ImageFile::IsAsset() const
{
    return false;
}

int ImageFile::GetWidth() const
{
    return m_width;
}

int ImageFile::GetHeight() const
{
    return m_height;
}

int ImageFile::GetNumComponents() const
{
    return m_numComponents;
}
