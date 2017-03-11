#include "Asset.h"

#include "FileReader.h"
#include "IO.h"
#include "AssetsManager.h"

Asset::Asset() : SerializableObject()
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
    Read(xmlInfo);
}

void Asset::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    Write(xmlInfo);
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

void Asset::Read(const XMLNode *xmlInfo)
{
    SerializableObject::Read(xmlInfo);
}

void Asset::Write(XMLNode *xmlInfo) const
{
    SerializableObject::Write(xmlInfo);
    xmlInfo->SetTagName("Asset");
}
