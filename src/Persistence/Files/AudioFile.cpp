#include "AudioFile.h"

#include "IO.h"

#ifdef BANG_EDITOR
#include "AudioFileInspectable.h"
#endif

AudioFile::AudioFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{

}

QPixmap AudioFile::GetIcon() const
{
    String fp = IO::ToAbsolute("./Icons/AudioIcon.png", true);
    QPixmap pm(fp.ToQString());
    return pm;
}

#ifdef BANG_EDITOR
SerializableObject *AudioFile::GetInspectable() const
{
    return new AudioFileInspectable(*this);
}
#endif
