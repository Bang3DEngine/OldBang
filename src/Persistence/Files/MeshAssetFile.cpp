#include "Bang/MeshAssetFile.h"

#include "Bang/IO.h"
#include "Bang/Mesh.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"

#ifdef BANG_EDITOR
#include "Bang/SerializableObject.h"
#include "Bang/MeshAssetFileInspectable.h"
#endif

MeshAssetFile::MeshAssetFile()
{
}

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

const QPixmap& MeshAssetFile::GetIcon() const
{
    String path = IO::ToAbsolute("./Icons/MeshAssetIcon.png", true);
    return IconManager::LoadPixmap(path, IconManager::IconOverlay::Asset);
}

#ifdef BANG_EDITOR
SerializableObject *MeshAssetFile::GetNewInspectable() const
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
