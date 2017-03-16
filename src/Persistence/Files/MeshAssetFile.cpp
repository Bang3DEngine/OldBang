#include "MeshAssetFile.h"

#include "Mesh.h"
#include "IO.h"
#include "AssetsManager.h"

#ifdef BANG_EDITOR
#include "SerializableObject.h"
#include "MeshAssetFileInspectable.h"
#endif

MeshAssetFile::MeshAssetFile(
        const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
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
    String fp = IO::ToAbsolute("./Icons/MeshAssetIcon.png", true);
    QPixmap pm(fp.ToQString());
    return pm;
}

#ifdef BANG_EDITOR
SerializableObject *MeshAssetFile::GetInspectable() const
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
