#include "FileMeshAsset.h"

#include "Mesh.h"
#include "AssetsManager.h"

FileMeshAsset::FileMeshAsset(const QFileSystemModel *model, const QModelIndex *index)
    : File(model, index)
{
}

const std::string& FileMeshAsset::GetMeshFilepath() const
{
    return m_meshFilepath;
}
