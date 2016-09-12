#include "ImageFile.h"

#ifdef BANG_EDITOR
#include "ImageFileInspectable.h"
#endif

ImageFile::ImageFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
    stbi_info(m_path.ToCString(), &m_width, &m_height, &m_numComponents);
}

QPixmap ImageFile::GetIcon() const
{
    String fp = GetAbsolutePath();

    // Mini cache
    static Map<std::string, QPixmap> filepath_To_Pixmap;
    if (!filepath_To_Pixmap.ContainsKey(fp))
    {
        QPixmap pm(QString::fromStdString(fp));
        filepath_To_Pixmap[fp] = pm;
    }

    return filepath_To_Pixmap[fp];
}

#ifdef BANG_EDITOR
IInspectable *ImageFile::GetInspectable() const
{
    return new ImageFileInspectable(*this);
}
#endif

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
