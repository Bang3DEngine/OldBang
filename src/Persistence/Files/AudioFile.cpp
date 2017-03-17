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
    return IconManager::LoadPixmap("./Icons/AudioIcon.png",
                                   IconManager::IconOverlay::Data, true);
}

#ifdef BANG_EDITOR
SerializableObject *AudioFile::GetInspectable() const
{
    return new AudioFileInspectable(*this);
}
#endif
