#include "AudioFile.h"

#include "Persistence.h"
#include "AudioFileInspectable.h"

AudioFile::AudioFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{

}

QPixmap AudioFile::GetIcon() const
{
    String fp = Persistence::ToAbsolute("./Icons/AudioIcon.png", true);
    QPixmap pm(fp.ToQString());
    return pm;
}

IInspectable *AudioFile::GetInspectable() const
{
    return new AudioFileInspectable(*this);
}
