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
    IResource* p_resource = nullptr;

    IResourceHandle();
    virtual ~IResourceHandle();

    IResource* Get() const;
    void Set(IResource* resource);

    friend class Resources;
};

template <class IResourceClass>
class ResourceHandle : public IResourceHandle
{
public:
    ResourceHandle()
    {
        m_typeId = GetTypeId<IResourceClass>();
    }

    virtual ~ResourceHandle() {}

    IResourceClass* Get() const
    {
        IResource *res = IResourceHandle::Get();
        return res ? SCAST<IResourceClass*>(res) : nullptr;
    }

    void Set(IResourceClass* resource)
    {
        IResourceHandle::Set( resource ? SCAST<IResource*>(resource) : nullptr);
    }

    friend class Resources;
};

template<class T>
using RH = ResourceHandle<T>;

NAMESPACE_BANG_END

#endif // RESOURCEHANDLE_H

