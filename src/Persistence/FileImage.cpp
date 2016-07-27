#include "FileImage.h"

FileImage::FileImage(const QFileSystemModel *model, const QModelIndex *index)
    : File(model, index)
{
    stbi_info(m_path.c_str(), &m_width, &m_height, &m_numComponents);
}

int FileImage::GetWidth() const
{
    return m_width;
}

int FileImage::GetHeight() const
{
    return m_height;
}

int FileImage::GetNumComponents() const
{
    return m_numComponents;
}
