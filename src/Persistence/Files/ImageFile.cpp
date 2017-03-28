#include "Bang/ImageFile.h"

#include "Bang/stb_image.h"

#include "Bang/Debug.h"
#include "Bang/IconManager.h"

#ifdef BANG_EDITOR
#include "Bang/ImageFileInspectable.h"
#endif

ImageFile::ImageFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
    stbi_info(m_path.ToCString(), &m_width, &m_height, &m_numComponents);
}

const QPixmap& ImageFile::GetIcon() const
{
    return IconManager::LoadPixmap(GetAbsolutePath(),
                                   IconManager::IconOverlay::Data);
}

#ifdef BANG_EDITOR
SerializableObject *ImageFile::GetInspectable() const
{
    return new ImageFileInspectable(*this);
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
