#include "Bang/Resources.h"

#include "Bang/File.h"
#include "Bang/Asset.h"
#include "Bang/Debug.h"
#include "Bang/Paths.h"
#include "Bang/Model.h"
#include "Bang/Object.h"
#include "Bang/Random.h"
#include "Bang/Window.h"
#include "Bang/Extensions.h"
#include "Bang/MeshFactory.h"
#include "Bang/ObjectManager.h"
#include "Bang/MaterialFactory.h"
#include "Bang/ShaderProgramFactory.h"

USING_NAMESPACE_BANG

Resources::Resources()
{
    m_meshFactory = new MeshFactory();
    m_materialFactory = new MaterialFactory();
    m_shaderProgramFactory = new ShaderProgramFactory();
}

Resources::~Resources()
{
    ASSERT_MSG(m_meshFactory == nullptr,          "Call Destroy()");
    ASSERT_MSG(m_materialFactory == nullptr,      "Call Destroy()");
    ASSERT_MSG(m_shaderProgramFactory == nullptr, "Call Destroy()");
}

RH<Resource> Resources::LoadFromExtension(const Path &filepath)
{
    RH<Resource> resRH;

    if (filepath.HasExtension(Extensions::GetModelExtensions()))
    {
        resRH.Set( Resources::Load<Model>(filepath).Get() );
    }

    return resRH;
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

void Resources::SetPermanent(IResource *resource, bool permanent)
{
    if (resource)
    {
        Resources *rs = Resources::GetActive(); ASSERT(rs);
        if (permanent) { rs->m_permanentResources.Add(resource); }
        else { rs->m_permanentResources.Remove(resource); }
    }
}

bool Resources::IsPermanent(IResource *resource)
{
    if (!resource) { return false; }

    Resources *rs = Resources::GetActive(); ASSERT(rs);
    return rs->m_permanentResources.Contains(resource);
}

void Resources::SetPermanent(const Path &resourcePath, bool permanent)
{
    if (!resourcePath.IsEmpty())
    {
        Resources *rs = Resources::GetActive(); ASSERT(rs);
        if (permanent) { rs->m_permanentResourcesPaths.Add(resourcePath); }
        else { rs->m_permanentResourcesPaths.Remove(resourcePath); }
    }
}

bool Resources::IsPermanent(const Path &resourcePath)
{
    if (resourcePath.IsEmpty()) { return false; }

    Resources *rs = Resources::GetActive(); ASSERT(rs);
    return rs->m_permanentResourcesPaths.Contains(resourcePath);
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

    Destroy(resEntry.resource);

    map.Remove(it);
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
    ++rs->m_GUIDCache.Get(resTypeId).Get(guid).usageCount;
}

void Resources::UnRegisterResourceUsage(const TypeId &resTypeId,
                                        IResource *resource)
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
/*
        bool stillBeingUsed = false;
        for (auto &pair : rs->m_GUIDCache)
        { if (pair.second.ContainsKey(guid)) { stillBeingUsed = true; break; } }

        if (!stillBeingUsed)
            */
        if (*resourcesUsage == 0)
        {
            const Path resourcePath = Resources::GetResourcePath(resource);
            if (!Resources::IsPermanent(resource) &&
                !Resources::IsPermanent(resourcePath) )
            {
                Resources::Remove(resTypeId, guid);
            }
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

Path Resources::GetResourcePath(IResource *resource)
{
    return ImportFilesManager::GetFilepath(resource->GetGUID());
}

String Resources::ToString(IResource *resource)
{
    String result = "";
    Resources *rs = Resources::GetActive();
    for (const auto &pair : rs->m_GUIDCache)
    {
        const String typeName = pair.first;
        for (const auto &pairMap : pair.second)
        {
            if (!resource || resource == pairMap.second.resource)
            {
                Path resourcePath = ImportFilesManager::GetFilepath(pairMap.first);
                result += typeName + ":";
                result += "  " + resourcePath +
                          "(" + String(pairMap.second.resource) + ", " +
                                String(pairMap.second.usageCount) +
                           ")";
                if (!resource) { result += "\n"; }
            }
        }
    }
    return result;
}

String Resources::ToString()
{
    return Resources::ToString(nullptr);
}

void Resources::Destroy()
{
    delete m_meshFactory;          m_meshFactory          = nullptr;
    delete m_materialFactory;      m_materialFactory      = nullptr;
    delete m_shaderProgramFactory; m_shaderProgramFactory = nullptr;
}

Resources *Resources::GetActive()
{
    Window *win = Window::GetActive();
    return  win ? win->GetResources() : nullptr;
}
