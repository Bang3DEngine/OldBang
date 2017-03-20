#include "AudioFile.h"

#include "IO.h"
#include "IconManager.h"

#ifdef BANG_EDITOR
#include "AudioFileInspectable.h"
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
