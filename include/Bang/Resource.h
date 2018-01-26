#ifndef RESOURCE_H
#define RESOURCE_H

#include "Bang/IGUIDable.h"
#include "Bang/Serializable.h"

NAMESPACE_BANG_BEGIN

#define RESOURCE_NO_CLONEABLE(CLASSNAME) \
    friend class Resources; \
    SERIALIZABLE(CLASSNAME)

#define RESOURCE(CLASSNAME) \
    RESOURCE_NO_CLONEABLE(CLASSNAME) \
    ICLONEABLE(CLASSNAME)

class Resource : public virtual Serializable,
                 public virtual IGUIDable
{
public:
    Path GetResourceFilepath() const;
    virtual void Import(const Path &resourceFilepath) = 0;

    virtual GUID::GUIDType GetNextInsideFileGUID() const;

protected:
    Resource();
    virtual ~Resource();

    Resource* GetInsideFileResource(const GUID &insideFileGUID) const;
    virtual Resource* GetInsideFileResource(GUID::GUIDType insideFileGUID) const;
    virtual String GetInsideFileResourceName(GUID::GUIDType insideFileGUID) const;

    virtual void ImportXML(const XMLNode &xmlInfo);
    virtual void ExportXML(XMLNode *xmlInfo) const;

    friend class Resources;
};

NAMESPACE_BANG_END

#endif // RESOURCE_H
