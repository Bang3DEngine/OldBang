#include "Bang/Resources.h"

#include "Bang/File.h"
#include "Bang/Asset.h"
#include "Bang/Paths.h"
#include "Bang/Object.h"
#include "Bang/Random.h"
#include "Bang/Window.h"
#include "Bang/ObjectManager.h"
#include "Bang/ShaderProgramFactory.h"

USING_NAMESPACE_BANG

Resources::Resources()
{
    m_shaderProgramFactory = new ShaderProgramFactory();
}

Resources::~Resources()
{
}

Array<IResource*> Resources::GetAllResources()
{
    Array<IResource*> result;
    Resources *rs = Resources::GetActive();
    for (auto& itMap : rs->m_GUIDCache)
    {
        for (const auto& it : itMap.second) { result.PushBack(it.second.resource); }
    }
    return result;
}

void Resources::Add(const TypeId &resTypeId, IResource *res)
{
    const GUID &guid = res->GetGUID();
    ASSERT(res != nullptr);
    ASSERT(!guid.IsEmpty());
    ASSERT(!resTypeId.IsEmpty());

    Resources *rs = Resources::GetActive(); ASSERT(rs);
    ASSERT(!Resources::Contains(resTypeId, guid));

    if (!rs->m_GUIDCache.ContainsKey(resTypeId))
    { rs->m_GUIDCache.Add(resTypeId); }

    ResourceEntry resourceEntry;
    resourceEntry.resource = res;
    resourceEntry.usageCount = 0;
    ASSERT(!rs->m_GUIDCache.Get(resTypeId).ContainsKey(guid));
    rs->m_GUIDCache.Get(resTypeId).Add(guid, resourceEntry);
}

void Resources::Remove(const TypeId &resTypeId, const GUID &guid)
{
    Resources *rs = Resources::GetActive(); ASSERT(rs);
    ASSERT(rs->m_GUIDCache.ContainsKey(resTypeId));

    auto &map = rs->m_GUIDCache.Get(resTypeId);
    ASSERT(map.ContainsKey(guid));

    auto it = map.Find(guid);
    const ResourceEntry &resEntry = it->second;
    ASSERT(resEntry.resource != nullptr);
    ASSERT(resEntry.usageCount == 0);

    map.Remove(it);
    ASSERT(!map.ContainsKey(guid));

    Destroy(resEntry.resource);
}

bool Resources::Contains(const TypeId &resTypeId, const GUID &guid)
{
    return Resources::GetCached(resTypeId, guid) != nullptr;
}

#ifdef DEBUG
bool Resources::_AssertCreatedFromResources = false;
bool Resources::_AssertDestroyedFromResources = false;

bool Resources::AssertCreatedFromResources()
{
    return !Resources::GetActive() || Resources::_AssertCreatedFromResources;
}

bool Resources::AssertDestroyedFromResources()
{
    return !Resources::GetActive() || Resources::_AssertDestroyedFromResources;
}
#endif

void Resources::RegisterResourceUsage(const TypeId &resTypeId, IResource *resource)
{
    const GUID &guid = resource->GetGUID();
    ASSERT(!guid.IsEmpty());
    ASSERT(!resTypeId.IsEmpty());

    Resources *rs = Resources::GetActive();
    if (!Resources::Contains(resTypeId, guid))
    {
        Resources::Add(resTypeId, resource);
    }
    ++rs->m_GUIDCache.Get(resTypeId)[guid].usageCount;
}

void Resources::UnRegisterResourceUsage(const TypeId &resTypeId, IResource *resource)
{
    const GUID &guid = resource->GetGUID();
    ASSERT(!guid.IsEmpty());
    ASSERT(!resTypeId.IsEmpty());

    Resources *rs = Resources::GetActive();
    if (rs)
    {
        ASSERT(Resources::Contains(resTypeId, guid));
        uint *resourcesUsage = &(rs->m_GUIDCache.Get(resTypeId)
                                 .Get(guid).usageCount);
        ASSERT(*resourcesUsage >= 1);

        --(*resourcesUsage);

        if (*resourcesUsage == 0 && !resource->m_isPersistentResource)
        {
            Resources::Remove(resTypeId, guid);
        }
    }
}

void Resources::Destroy(IResource *resource)
{
    #ifdef DEBUG
    Resources::_AssertDestroyedFromResources = true;
    #endif

    Asset *asset = Cast<Asset*>(resource);
    if (asset) { Asset::Destroy(asset); } else { delete resource; }

    #ifdef DEBUG
    Resources::_AssertDestroyedFromResources = false;
    #endif
}

IResource *Resources::GetCached(const TypeId &resTypeId, const GUID &guid)
{
    Resources *rs = Resources::GetActive();
    if (!rs->m_GUIDCache.ContainsKey(resTypeId)) { return nullptr; }
    if (!rs->m_GUIDCache.Get(resTypeId).ContainsKey(guid)) { return nullptr; }
    return rs->m_GUIDCache.Get(resTypeId).Get(guid).resource;
}

Resources *Resources::GetActive()
{
    Window *win = Window::GetActive();
    return  win ? win->GetResources() : nullptr;
}
