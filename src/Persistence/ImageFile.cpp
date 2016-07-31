#include "ImageFile.h"

ImageFile::ImageFile(const QFileSystemModel *model, const QModelIndex *index)
    : File(model, index)
{
    stbi_info(m_path.c_str(), &m_width, &m_height, &m_numComponents);
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
