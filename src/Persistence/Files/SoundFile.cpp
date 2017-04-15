#include "Bang/SoundFile.h"

#include "Bang/IO.h"
#include "Bang/IconManager.h"

#ifdef BANG_EDITOR
#include "Bang/SoundFileInspectable.h"
#endif

SoundFile::SoundFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{

}

const QPixmap& SoundFile::GetIcon() const
{
    String path = IO::ToAbsolute("./Icons/AudioIcon.png", true);
    return IconManager::LoadPixmap(path, IconManager::IconOverlay::Data);
}

#ifdef BANG_EDITOR
SerializableObject *SoundFile::GetInspectable() const
{
    return new SoundFileInspectable(*this);
}
#endif
