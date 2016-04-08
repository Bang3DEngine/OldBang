#ifndef FILEIMAGE_H
#define FILEIMAGE_H

#include "stb_image.h"
#include "Logger.h"

#include "File.h"

class FileImage : public File
{
private:
    int width = 0, height = 0;
    int numComponents = 3;

public:
    FileImage(const QFileSystemModel *model, const QModelIndex *index);

    int GetWidth() const;
    int GetHeight() const;
    int GetNumComponents() const;
};

#endif // FILEIMAGE_H
