#include "Bang/Asset.h"

#include "Bang/IO.h"
#include "Bang/FileReader.h"
#include "Bang/AssetsManager.h"

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

String Asset::GetFileExtension() const
{
    return Asset::GetFileExtensionStatic();
}

const Path& Asset::GetFilepath() const
{
    return m_assetFilepath;
}

void Asset::CloneInto(ICloneable * clone) const
{
}

String Asset::ToString() const
{
    return "Asset '" + m_assetFilepath + "'.";
}

void Asset::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);
}

void Asset::Write(XMLNode *xmlInfo) const
{
    SerializableObject::Write(xmlInfo);
}
