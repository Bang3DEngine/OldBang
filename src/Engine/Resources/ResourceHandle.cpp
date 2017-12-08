#include "Bang/ResourceHandle.h"

#include "Bang/Resources.h"

USING_NAMESPACE_BANG

IResourceHandle::IResourceHandle()
{
    // Debug_Log("Construct RH " << Get());
    // Debug_Log(Resources::GetActive()->m_GUIDCache);
}

IResourceHandle::IResourceHandle(const IResourceHandle &rhs)
{
    // Debug_Log("Copy constructor begin");
    // Debug_Log(Resources::GetActive()->m_GUIDCache);
    *this = rhs;
    // Debug_Log("Copy constructor end");
    // Debug_Log(Resources::GetActive()->m_GUIDCache);
}
IResourceHandle::IResourceHandle(IResourceHandle &&rhs) { *this = rhs; }
IResourceHandle& IResourceHandle::operator=(const IResourceHandle &rhs)
{
    // Debug_Log("operator= begin on " << rhs.Get());
    // Debug_Log(Resources::GetActive()->m_GUIDCache);
    if (&rhs != this)
    {
        m_typeId = rhs.m_typeId;
        Set(rhs.Get());
    }
    // Debug_Log("operator= end on " << rhs.Get());
    // Debug_Log(Resources::GetActive()->m_GUIDCache);
    return *this;
}
IResourceHandle& IResourceHandle::operator=(IResourceHandle &&rhs)
{
    // Debug_Log("MOVE operator= begin on " << rhs.Get());
    // Debug_Log(Resources::GetActive()->m_GUIDCache);
    if (&rhs != this)
    {
        // Swap resources without triggering Resources usageCounting, since
        // usageCount will remain the same.polymo
        rhs.p_resource = nullptr;
        p_resource = Get();
        std::swap(m_typeId, rhs.m_typeId);
    }
    // Debug_Log("MOVE operator= end on " << rhs.Get());
    // Debug_Log(Resources::GetActive()->m_GUIDCache);
    return *this;
}

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
    // Debug_Log("Destruct RH begin " << Get());
    // Debug_Log(Resources::GetActive()->m_GUIDCache);
    Set(nullptr);
    // Debug_Log("Destruct RH end " << Get());
    // Debug_Log(Resources::GetActive()->m_GUIDCache);
}

IResource *IResourceHandle::Get() const { return p_resource; }
void IResourceHandle::Set(IResource *resource)
{
    // Debug_Log("Set RH begin from " << Get() << " to " << resource);
    // Debug_Log(Resources::GetActive()->m_GUIDCache);
    if (Get() != resource)
    {
        if (Get())
        {
            Resources::UnRegisterResourceUsage( m_typeId, Get() );
        }

        p_resource = resource;
        if (Get())
        {
            Resources::RegisterResourceUsage( m_typeId, Get() );
        }
    }
    // Debug_Log("Set RH end to " << Get());
    // Debug_Log(Resources::GetActive()->m_GUIDCache);
}
