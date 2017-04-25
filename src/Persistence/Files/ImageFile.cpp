#include "Bang/ImageFile.h"

#include "Bang/Debug.h"
#include "Bang/Image.h"
#include "Bang/IconManager.h"
#include "Bang/Inspectable.h"

#ifdef BANG_EDITOR
#include "Bang/ImageFileInspectable.h"
#endif

ImageFile::ImageFile()
{
}

ImageFile::ImageFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
    QImage img = IconManager::LoadImage(GetAbsolutePath().ToQString());
    m_width  = img.width();
    m_height = img.height();
    m_numComponents = 4;
}

const QPixmap& ImageFile::GetIcon() const
{
    return IconManager::LoadPixmap(GetAbsolutePath(),
                                   IconManager::IconOverlay::Data);
}

#ifdef BANG_EDITOR
IInspectable *ImageFile::GetNewInspectable() const
{
    return nullptr; //new Inspectable<ImageFileInspectable>(*this);
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
