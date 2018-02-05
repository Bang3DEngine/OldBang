#ifndef RESOURCE_H
#define RESOURCE_H

#include "Bang/IGUIDable.h"
#include "Bang/Serializable.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

#define RESOURCE(CLASSNAME) \
    SERIALIZABLE(CLASSNAME) \
    friend class Resources;

FORWARD class Resource;
class IResourceListener : public virtual IEventListener
{
    EVENTLISTENER(IResourceListener)

public:
    virtual void OnImported(Resource *res) { (void)(res); }
};

class Resource : public Serializable,
                 public EventEmitter<IResourceListener>
{
public:
    Path GetResourceFilepath() const;

    virtual GUID::GUIDType GetNextInsideFileGUID() const;

protected:
    Resource();
    virtual ~Resource();

    Resource* GetInsideFileResource(const GUID &insideFileGUID) const;
    virtual Resource* GetInsideFileResource(GUID::GUIDType insideFileGUID) const;
    virtual String GetInsideFileResourceName(GUID::GUIDType insideFileGUID) const;

    virtual void ImportXML(const XMLNode &xmlInfo);
    virtual void ExportXML(XMLNode *xmlInfo) const;

    void _Import(const Path &resourceFilepath);
    virtual void Import(const Path &resourceFilepath) = 0;

    friend class Resources;
};

NAMESPACE_BANG_END

#endif // RESOURCE_H
