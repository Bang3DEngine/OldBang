#include "MeshFile.h"

#include "Debug.h"

#include "FileReader.h"
#include "Persistence.h"

#ifdef BANG_EDITOR
#include "IInspectable.h"
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
    String fp = Persistence::ToAbsolute("./Assets/Engine/Icons/MeshAssetIcon.png");
    QPixmap pm(QString::fromStdString(fp));
    return pm;
}

#ifdef BANG_EDITOR
IInspectable *MeshFile::GetInspectable() const
{
    return new MeshFileInspectable(*this);
}
#endif

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
