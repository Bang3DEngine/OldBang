#include "MeshAssetFile.h"

#include "Mesh.h"
#include "AssetsManager.h"
#include "MeshAssetFileInspectable.h"

MeshAssetFile::MeshAssetFile(const QFileSystemModel *model, const QModelIndex *index)
    : File(model, index)
{
    XMLNode *xmlInfo;
    xmlInfo = XMLParser::FromFile(m_path);
    m_meshFilepath = xmlInfo->GetFilepath("MeshFilepath");
    delete xmlInfo;
}

QPixmap MeshAssetFile::GetIcon() const
{
    std::string fp = Persistence::ToAbsolute("./Assets/Engine/Icons/MeshAssetIcon.png");
    QPixmap pm(QString::fromStdString(fp));
    return pm;
}

IInspectable *MeshAssetFile::GetInspectable() const
{
    return new MeshAssetFileInspectable(*this);
}

const std::string& MeshAssetFile::GetMeshFilepath() const
{
    return m_meshFilepath;
}
