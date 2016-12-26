#ifndef FILETEXTURE2DASSET_H
#define FILETEXTURE2DASSET_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>

#include "File.h"

class IInspectable;
class Texture2DAssetFile : public File
{
public:
    Texture2DAssetFile(const QFileSystemModel *model, const QModelIndex &index);

    String GetImageFilepath() const;
    virtual QPixmap GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetInspectable() const override;
    #endif
};

#endif // FILETEXTURE2DASSET_H
