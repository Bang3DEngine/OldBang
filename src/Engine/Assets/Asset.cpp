#include "Bang/Asset.h"

#include "Bang/Paths.h"
#include "Bang/Resources.h"

Asset::Asset()
{
}

Asset::~Asset()
{
    Resources::UnLoad(this);
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
    String res = GetClassName();
    res +=  " '" + m_assetFilepath + "'. (" +
            String::ToString((void*)this) + ")";
    return res;
}

void Asset::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);
}

void Asset::Write(XMLNode *xmlInfo) const
{
    SerializableObject::Write(xmlInfo);
}
