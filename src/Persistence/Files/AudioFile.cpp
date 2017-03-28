#include "Bang/AudioFile.h"

#include "Bang/IO.h"
#include "Bang/IconManager.h"

#ifdef BANG_EDITOR
#include "Bang/AudioFileInspectable.h"
#endif

AudioFile::AudioFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{

}

const QPixmap& AudioFile::GetIcon() const
{
    String path = IO::ToAbsolute("./Icons/AudioIcon.png", true);
    return IconManager::LoadPixmap(path, IconManager::IconOverlay::Data);
}

#ifdef BANG_EDITOR
SerializableObject *AudioFile::GetInspectable() const
{
    return new AudioFileInspectable(*this);
}
#endif
