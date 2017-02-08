#include "Asset.h"

#include "FileWriter.h"
#include "FileReader.h"
#include "Persistence.h"
#include "AssetsManager.h"

Asset::Asset()
{

}

Asset::~Asset()
{
    AssetsManager::Unload(this);
}


String Asset::GetFileExtensionStatic()
{
    return "basset";
}

String Asset::GetFileExtension()
{
    return Asset::GetFileExtensionStatic();
}

#ifdef BANG_EDITOR
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
    return m_assetFilepath;
}

String Asset::ToString() const
{
    return "Asset '" + m_assetFilepath + "'.";
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
