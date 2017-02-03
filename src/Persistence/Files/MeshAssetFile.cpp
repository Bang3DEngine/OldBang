#include "MeshAssetFile.h"

#include "Mesh.h"
#include "Persistence.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "IInspectable.h"
#include "MeshAssetFileInspectable.h"
#endif

MeshAssetFile::MeshAssetFile(
        const QFileSystemModel *model, const QModelIndex &index) : File(model, index)
{
    XMLNode *xmlInfo;
    xmlInfo = XMLParser::FromFile(m_path);
    if (xmlInfo)
    {
        m_meshFilepath = xmlInfo->GetFilepath("MeshFilepath");
        delete xmlInfo;
    }
}

QPixmap MeshAssetFile::GetIcon() const
{
    String fp = Persistence::ToAbsolute("./Icons/MeshAssetIcon.png", true);
    QPixmap pm(fp.ToQString());
    return pm;
}

#ifdef BANG_EDITOR
IInspectable *MeshAssetFile::GetInspectable() const
{
    return new MeshAssetFileInspectable(*this);
}

void MeshAssetFile::SetMeshFilepath(const String &meshFilepath)
{
    m_meshFilepath = meshFilepath;
}

#endif

bool MeshAssetFile::IsAsset() const
{
    return true;
}

const String& MeshAssetFile::GetMeshFilepath() const
{
    return m_meshFilepath;
}
