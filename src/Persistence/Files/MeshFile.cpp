#include "Bang/MeshFile.h"

#include "Bang/Debug.h"

#include "Bang/IO.h"
#include "Bang/FileReader.h"
#include "Bang/IconManager.h"

#ifdef BANG_EDITOR
#include "Bang/SerializableObject.h"
#include "Bang/MeshFileInspectable.h"
#endif

MeshFile::MeshFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
    m_numFaces = FileReader::GetMeshNumTriangles(m_path);
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

int MeshFile::GetNumFaces() const
{
    return m_numFaces;
}
