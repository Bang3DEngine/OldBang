#include "Bang/ImageFile.h"

#include "Bang/Debug.h"
#include "Bang/G_Image.h"
#include "Bang/IconManager.h"

ImageFile::ImageFile()
{
}

ImageFile::ImageFile(const Path& path)
    : BFile(path)
{
    QImage img = IconManager::LoadImage(GetPath());
    m_width  = img.width();
    m_height = img.height();
    m_numComponents = 4;
}

const QPixmap& ImageFile::GetIcon() const
{
    return IconManager::LoadPixmap(GetPath(), IconManager::IconOverlay::Data);
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
