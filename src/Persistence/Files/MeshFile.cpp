#include "MeshFile.h"

#include "Debug.h"

#include "FileReader.h"
#include "IO.h"

#ifdef BANG_EDITOR
#include "SerializableObject.h"
#include "MeshFileInspectable.h"
#endif

MeshFile::MeshFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
    FileReader::GetOBJFormat(m_path, &m_hasUvs, &m_hasNormals, &m_isTris);
    m_numFaces = FileReader::GetOBJNumFaces(m_path);
}

QPixmap MeshFile::GetIcon() const
{
    String fp = IO::ToAbsolute("./Icons/MeshAssetIcon.png", true);
    QPixmap pm(fp.ToQString());
    return pm;
}

#ifdef BANG_EDITOR
SerializableObject *MeshFile::GetInspectable() const
{
    return new MeshFileInspectable(*this);
}
#endif

bool MeshFile::IsAsset() const
{
    return false;
}

bool MeshFile::IsTriangles() const
{
    return m_isTris;
}

bool MeshFile::HasUvs() const
{
    return m_hasUvs;
}

bool MeshFile::HasNormals() const
{
    return m_hasNormals;
}

int MeshFile::GetNumFaces() const
{
    return m_numFaces;
}
