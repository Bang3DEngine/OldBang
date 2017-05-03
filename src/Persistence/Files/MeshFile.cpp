#include "Bang/MeshFile.h"

#include "Bang/Paths.h"
#include "Bang/Mesh.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/FileInspectable.h"

MeshFile::MeshFile()
{
}

MeshFile::MeshFile(const Path& path)
    : File(path)
{
    XMLNode xmlInfo = XMLParser::FromFile(GetPath());
    m_modelFilepath = xmlInfo.GetFilepath("ModelFilepath");
}

const QPixmap& MeshFile::GetIcon() const
{
    return IconManager::LoadPixmap(EPATH("Icons/MeshIcon.png"),
                                   IconManager::IconOverlay::Asset);
}

void MeshFile::Read(const XMLNode &xmlInfo)
{
    SetModelFilepath( xmlInfo.GetFilepath("ModelFilepath") );

    #ifdef BANG_EDITOR
    AssetsManager::UpdateAsset(GetPath(), xmlInfo);
    #endif
}

void MeshFile::Write(XMLNode *xmlInfo) const
{
    File::Write(xmlInfo);
    xmlInfo->SetFilepath("ModelFilepath", GetModelFilepath(), "obj stl mb fbx");
}

#ifdef BANG_EDITOR
IInspectable *MeshFile::GetNewInspectable()
{
    return new FileInspectable<MeshFile>(*this);
}
#endif

void MeshFile::SetModelFilepath(const Path &modelFilepath)
{
    m_modelFilepath = modelFilepath;
}

bool MeshFile::IsAsset() const
{
    return true;
}

const Path& MeshFile::GetModelFilepath() const
{
    return m_modelFilepath;
}
