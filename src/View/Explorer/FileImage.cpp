#include "FileImage.h"

FileImage::FileImage(const QFileSystemModel *model, const QModelIndex *index)
    : File(model, index)
{
    stbi_info(path.c_str(), &width, &height, &numComponents);
}

int FileImage::GetWidth() const
{
    return width;
}

int FileImage::GetHeight() const
{
    return height;
}

int FileImage::GetNumComponents() const
{
    return numComponents;
}
