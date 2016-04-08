#ifndef FILEMESH_H
#define FILEMESH_H

#include "FileReader.h"
#include "Logger.h"

#include "File.h"

class FileMesh : public File
{
    bool isTris = true, hasUvs = false, hasNormals = false;
    int numFaces = 0;

public:
    FileMesh(const QFileSystemModel *model, const QModelIndex *index);

    bool IsTriangles() const;
    bool HasUvs() const;
    bool HasNormals() const;
    int GetNumFaces() const;
};

#endif // FILEMESH_H
#ifndef FILEIMAGE_H
#define FILEIMAGE_H


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
