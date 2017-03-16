#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>

#include "File.h"

class AudioFile : public File
{
public:
    AudioFile(const QFileSystemModel *model, const QModelIndex &index);

    virtual QPixmap GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual SerializableObject *GetInspectable() const override;
    #endif
};

#endif // AUDIOFILE_H
