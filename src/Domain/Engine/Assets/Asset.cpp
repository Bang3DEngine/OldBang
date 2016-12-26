#include "Asset.h"

#include "FileWriter.h"
#include "FileReader.h"
#include "AssetsManager.h"

Asset::Asset()
{

}

Asset::~Asset()
{
    AssetsManager::UnloadAsset(this);
}

#ifdef BANG_EDITOR

const String Asset::GetFileExtensionStatic()
{
    return "basset";
}

const String Asset::GetFileExtension()
{
    return Asset::GetFileExtensionStatic();
}

void Asset::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}

void Asset::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}
#endif

String Asset::GetFilepath() const
{
    return m_filepath;
}

const String Asset::ToString() const
{
    return "Asset '" + m_filepath + "'.";
}


void Asset::ReadXMLInfo(const XMLNode *xmlInfo)
{
    IFileable::ReadXMLInfo(xmlInfo);
}

void Asset::FillXMLInfo(XMLNode *xmlInfo) const
{
    IFileable::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Asset");
    //xmlInfo->SetFilepath("filepath", m_filepath);
}
