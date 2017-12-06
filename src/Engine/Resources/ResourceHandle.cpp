#include "Bang/ResourceHandle.h"

#include "Bang/Resources.h"

USING_NAMESPACE_BANG

IResourceHandle::IResourceHandle()
{}
/*
IResourceHandle::IResourceHandle(const IResourceHandle &rhs) { *this = rhs; }
IResourceHandle::IResourceHandle(IResourceHandle &&rhs) { *this = rhs; }
IResourceHandle& IResourceHandle::operator=(const IResourceHandle &rhs)
{
    if (&rhs != this) { Set(rhs.Get()); }
    return *this;
}
IResourceHandle& IResourceHandle::operator=(IResourceHandle &&rhs)
{
    if (&rhs != this)
    {
        rhs.p_resource = Get();
        p_resource = nullptr;
    }
    return *this;
}
*/
bool IResourceHandle::operator==(const IResourceHandle &rhs) const
{ return Get() == rhs.Get(); }
bool IResourceHandle::operator!=(const IResourceHandle &rhs) const
{ return !(*this == rhs); }
bool IResourceHandle::operator<(const IResourceHandle &rhs) const
{ return Get() < rhs.Get(); }
Bang::IResourceHandle::operator bool() const
{ return (Get() != nullptr); }

IResourceHandle::~IResourceHandle()
{
    Set(nullptr);
}

IResource *IResourceHandle::Get() const { return p_resource; }
void IResourceHandle::Set(IResource *resource)
{
    if (Get() != resource)
    {
        if (Get())
        {
            // Resources::UnRegisterResourceUsage( m_typeId, Get() );
        }

        p_resource = resource;
        if (Get())
        {
            Resources::RegisterResourceUsage( m_typeId, Get() );
        }
    }
}
