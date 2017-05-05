#include "Bang/ImageFile.h"

#include "Bang/Debug.h"
#include "Bang/Image.h"
#include "Bang/IconManager.h"
#include "Bang/FileInspectable.h"

ImageFile::ImageFile()
{
}

ImageFile::ImageFile(const Path& path)
    : File(path)
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
    File::Read(xmlInfo);
}

void ImageFile::Write(XMLNode *xmlInfo) const
{
    File::Write(xmlInfo);

    xmlInfo->SetString("FileName", GetPath().GetNameExt(),
                       {XMLProperty::Readonly});
    xmlInfo->SetFilepath("ImageFilePath", GetPath(), "",
                         {XMLProperty::Readonly});
    xmlInfo->SetString("Dimensions",
                       String(GetWidth()) + "x" + String(GetHeight()),
                       {XMLProperty::Readonly});
    xmlInfo->SetString("Mode", GetNumComponents() == 3 ? "RGB" : "RGBA",
                       {XMLProperty::Readonly});
}

#ifdef BANG_EDITOR
IInspectable *ImageFile::GetNewInspectable()
{
    return new FileInspectable<ImageFile>(*this);
}
#endif

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
