#include "MeshFile.h"

#include "Persistence.h"
#include "MeshFileInspectable.h"

MeshFile::MeshFile(const QFileSystemModel *model, const QModelIndex *index)
    : File(model, index)
{
    FileReader::GetOBJFormat(m_path, &m_hasUvs, &m_hasNormals, &m_isTris);
    m_numFaces = FileReader::GetOBJNumFaces(m_path);
}

QPixmap MeshFile::GetIcon() const
{
    std::string fp = Persistence::ToAbsolute("./Assets/Engine/Icons/MeshAssetIcon.png");
    QPixmap pm(QString::fromStdString(fp));
    return pm;
}

IInspectable *MeshFile::GetInspectable() const
{
    return new MeshFileInspectable(*this);
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
