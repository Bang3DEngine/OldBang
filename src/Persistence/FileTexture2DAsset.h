#ifndef FILETEXTURE2DASSET_H
#define FILETEXTURE2DASSET_H

#include "stb_image.h"
#include "Logger.h"

#include "File.h"

class FileTexture2DAsset : public File
{
public:
    FileTexture2DAsset(const QFileSystemModel *model, const QModelIndex *index);
};

#endif // FILETEXTURE2DASSET_H
