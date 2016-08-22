#include "ImageFile.h"

#include "ImageFileInspectable.h"

ImageFile::ImageFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
    stbi_info(m_path.ToCString(), &m_width, &m_height, &m_numComponents);
}

QPixmap ImageFile::GetIcon() const
{
    String fp = GetAbsolutePath();

    // Mini cache
    static std::map<std::string, QPixmap> filepath_To_pixmap;
    if (filepath_To_pixmap.find(fp) == filepath_To_pixmap.end())
    {
        QPixmap pm(QString::fromStdString(fp));
        filepath_To_pixmap[fp] = pm;
    }

    return filepath_To_pixmap[fp];
    // return File::AddNoAssetFileQPixmapOnTopOf(pm);
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
