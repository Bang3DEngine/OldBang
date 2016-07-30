#include "FileMesh.h"

FileMesh::FileMesh(const QFileSystemModel *model, const QModelIndex *index)
    : File(model, index)
{
    FileReader::GetOBJFormat(m_path, &m_hasUvs, &m_hasNormals, &m_isTris);
    m_numFaces = FileReader::GetOBJNumFaces(m_path);
}

bool FileMesh::IsTriangles() const
{
    return m_isTris;
}

bool FileMesh::HasUvs() const
{
    return m_hasUvs;
}

bool FileMesh::HasNormals() const
{
    return m_hasNormals;
}

int FileMesh::GetNumFaces() const
{
    return m_numFaces;
}
