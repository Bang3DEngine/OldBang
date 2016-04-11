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
