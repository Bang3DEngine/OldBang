#ifndef RESOURCEHANDLE_H
#define RESOURCEHANDLE_H

#include "Bang/TypeMap.h"
#include "Bang/Resource.h"

NAMESPACE_BANG_BEGIN

class IResourceHandle
{
public:
    IResourceHandle(const IResourceHandle &rhs);
    IResourceHandle(IResourceHandle &&rhs);
    IResourceHandle& operator=(IResourceHandle &&rhs);
    IResourceHandle& operator=(const IResourceHandle &rhs);

    bool operator==(const IResourceHandle &rhs) const;
    bool operator!=(const IResourceHandle &rhs) const;
    bool operator<(const IResourceHandle &rhs) const;
    operator bool() const;

protected:
    TypeId m_typeId;
    Resource* p_resource = nullptr;

    IResourceHandle();
    virtual ~IResourceHandle();

    Resource* Get() const;
    void Set(Resource* resource);

    friend class Resources;
};

template <class ResourceClass>
class ResourceHandle : public IResourceHandle
{
public:
    ResourceHandle()
    {
        m_typeId = GetTypeId<ResourceClass>();
    }

    ResourceHandle(ResourceClass *res) : ResourceHandle()
    {
        Set(res);
    }

    virtual ~ResourceHandle() {}

    ResourceClass* Get() const
    {
        Resource *res = IResourceHandle::Get();
        return res ? SCAST<ResourceClass*>(res) : nullptr;
    }

    void Set(ResourceClass* resource)
    {
        IResourceHandle::Set( resource ? SCAST<Resource*>(resource) : nullptr);
    }

    friend class Resources;
};

template<class T>
using RH = ResourceHandle<T>;

NAMESPACE_BANG_END

#endif // RESOURCEHANDLE_H

