#include "Bang/MeshFile.h"

#include "Bang/Mesh.h"
#include "Bang/Paths.h"
#include "Bang/AssetsManager.h"

MeshFile::MeshFile()
{
}

MeshFile::MeshFile(const Path& path)
    : BFile(path)
{
    XMLNode xmlInfo = XMLParser::FromFile(GetPath());
    m_modelFilepath = xmlInfo.GetFilepath("ModelFilepath");
}

void MeshFile::Read(const XMLNode &xmlInfo)
{
    BFile::Read(xmlInfo);
    SetModelFilepath( xmlInfo.GetFilepath("ModelFilepath") );
}

void MeshFile::Write(XMLNode *xmlInfo) const
{
    BFile::Write(xmlInfo);
    xmlInfo->SetFilepath("ModelFilepath", GetModelFilepath());
}

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
