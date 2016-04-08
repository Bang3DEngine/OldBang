#include "FileMesh.h"

FileMesh::FileMesh(const QFileSystemModel *model, const QModelIndex *index)
    :File(model, index)
{
    FileReader::GetOBJFormat(path, &hasUvs, &hasNormals, &isTris);
    numFaces = FileReader::GetOBJNumFaces(path);
}

bool FileMesh::IsTriangles() const
{
    return isTris;
}

bool FileMesh::HasUvs() const
{
    return hasUvs;
}

bool FileMesh::HasNormals() const
{
    return hasNormals;
}

int FileMesh::GetNumFaces() const
{
    return numFaces;
}
