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

void Asset::CloneInto(ICloneable * clone) const
{
    SerializableObject::CloneInto(clone);
}

String Asset::ToString() const
{
    String res = GetClassName();
    res +=  "Asset(" + String::ToString((void*)this) + ")";
    return res;
}

void Asset::Read(const XMLNode &xmlInfo)
{
    Resource::Read(xmlInfo);
}

void Asset::Write(XMLNode *xmlInfo) const
{
    Resource::Write(xmlInfo);
}
