#include "ImageFile.h"

#include "ImageFileInspectable.h"

ImageFile::ImageFile(const QFileSystemModel *model, const QModelIndex *index)
    : File(model, index)
{
    stbi_info(m_path.c_str(), &m_width, &m_height, &m_numComponents);
}

QPixmap ImageFile::GetIcon() const
{
    std::string fp = GetAbsolutePath();
    QPixmap pm(QString::fromStdString(fp));
    return File::AddNoAssetFileQPixmapOnTopOf(pm);
}

IInspectable *ImageFile::GetInspectable() const
{
    return new ImageFileInspectable(*this);
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
