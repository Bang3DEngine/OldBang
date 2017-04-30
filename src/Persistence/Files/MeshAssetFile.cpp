#include "Bang/MeshAssetFile.h"

#include "Bang/IO.h"
#include "Bang/Mesh.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/FileInspectable.h"

MeshAssetFile::MeshAssetFile()
{
}

MeshAssetFile::MeshAssetFile(
        const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
    XMLNode xmlInfo = XMLParser::FromFile(GetPath().GetAbsolute());
    m_meshFilepath = xmlInfo.GetFilepath("MeshFilepath");
}

const QPixmap& MeshAssetFile::GetIcon() const
{
    return IconManager::LoadPixmap(EPATH("Icons/MeshAssetIcon.png"),
                                   IconManager::IconOverlay::Asset);
}

void MeshAssetFile::Read(const XMLNode &xmlInfo)
{
    SetMeshFilepath( xmlInfo.GetFilepath("MeshFilepath") );
    AssetsManager::UpdateAsset(GetPath(), xmlInfo);
}

void MeshAssetFile::Write(XMLNode *xmlInfo) const
{
    File::Write(xmlInfo);
    xmlInfo->SetFilepath("MeshFilepath", GetMeshFilepath(), "obj stl mb fbx");
}

#ifdef BANG_EDITOR
IInspectable *MeshAssetFile::GetNewInspectable()
{
    return new FileInspectable<MeshAssetFile>(*this);
}

void MeshAssetFile::SetMeshFilepath(const Path &meshFilepath)
{
    m_meshFilepath = meshFilepath;
}
#endif

bool MeshAssetFile::IsAsset() const
{
    return true;
}

const Path& MeshAssetFile::GetMeshFilepath() const
{
    return m_meshFilepath;
}
