#ifndef FILETEXTURE2DASSET_H
#define FILETEXTURE2DASSET_H

#include "stb_image.h"
#include "Logger.h"

#include "File.h"

class Texture2DAssetFile : public File
{
public:
    Texture2DAssetFile(const QFileSystemModel *model, const QModelIndex &index);

    std::string GetImageFilepath() const;
    virtual QPixmap GetIcon() const override;
    virtual IInspectable *GetInspectable() const override;
};

#endif // FILETEXTURE2DASSET_H
