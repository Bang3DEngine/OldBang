#include "MeshFile.h"

#include "Debug.h"

#include "IO.h"
#include "FileReader.h"
#include "IconManager.h"

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

const QPixmap& MeshFile::GetIcon() const
{
    String path = IO::ToAbsolute("./Icons/MeshAssetIcon.png", true);
    return IconManager::LoadPixmap(path, IconManager::IconOverlay::Data);
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
