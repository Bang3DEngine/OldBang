#ifndef FILETEXTURE2DASSET_H
#define FILETEXTURE2DASSET_H

#include "stb_image.h"
#include "Debug.h"

#include "File.h"

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
