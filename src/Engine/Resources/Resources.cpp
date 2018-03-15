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
#include "Bang/MaterialFactory.h"
#include "Bang/ShaderProgramFactory.h"

USING_NAMESPACE_BANG

Resources::Resources()
{
    m_meshFactory = CreateMeshFactory();
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

void Resources::Import(Resource *res)
{
    res->_Import( res->GetResourceFilepath() );
}

Array<Resource*> Resources::GetAllResources()
{
    Array<Resource*> result;
    Resources *rs = Resources::GetActive();
    for (auto& itMap : rs->m_resourcesCache)
    {
        for (const auto& it : itMap.second) { result.PushBack(it.second.resource); }
    }
    return result;
}

void Resources::ExportXMLResource(const Resource *resource,
                                  const Path &exportFilepath)
{
    File::Write(exportFilepath, "");
    Path importFile = ImportFilesManager::CreateImportFile(exportFilepath).first;
    resource->ExportXMLToFile(importFile);
}

void Resources::Add(const TypeId &resTypeId, Resource *res)
{
    const GUID &guid = res->GetGUID();
    ASSERT(res != nullptr);
    ASSERT(!guid.IsEmpty());
    ASSERT(!resTypeId.IsEmpty());

    Resources *rs = Resources::GetActive(); ASSERT(rs);
    ASSERT(!Resources::Contains(resTypeId, guid));

    if (!rs->m_resourcesCache.ContainsKey(resTypeId))
    { rs->m_resourcesCache.Add(resTypeId); }

    ResourceEntry resourceEntry;
    resourceEntry.resource = res;
    resourceEntry.usageCount = 0;
    ASSERT(!rs->m_resourcesCache.Get(resTypeId).ContainsKey(guid));
    rs->m_resourcesCache.Get(resTypeId).Add(guid, resourceEntry);
}

void Resources::SetPermanent(Resource *resource, bool permanent)
{
    if (resource)
    {
        Resources *rs = Resources::GetActive(); ASSERT(rs);
        if (permanent) { rs->m_permanentResources.Add(resource); }
        else { rs->m_permanentResources.Remove(resource); }
    }
}

bool Resources::IsPermanent(Resource *resource)
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
    ASSERT(rs->m_resourcesCache.ContainsKey(resTypeId));

    auto &map = rs->m_resourcesCache.Get(resTypeId);
    ASSERT(map.ContainsKey(guid));

    auto it = map.Find(guid);
    ResourceEntry resEntry = it->second;
    map.Remove(it);

    ASSERT(resEntry.resource != nullptr);
    ASSERT(resEntry.usageCount == 0);

    bool totallyUnused = true;
    for (const auto &pair : rs->m_resourcesCache)
    {
        if (pair.second.ContainsKey(resEntry.resource->GetGUID()))
        {
            totallyUnused = false;
            break;
        }
    }

    if (totallyUnused) { Destroy(resEntry.resource); }
}

bool Resources::Contains(const TypeId &resTypeId, const GUID &guid)
{
    return Resources::GetCached(resTypeId, guid) != nullptr;
}

Array<Path> Resources::GetLookUpPaths() const
{
    return {Paths::GetProjectAssetsDir(), Paths::GetEngineAssetsDir()};
}

void Resources::RegisterResourceUsage(const TypeId &resTypeId, Resource *resource)
{
    const GUID &guid = resource->GetGUID();
    ASSERT(!guid.IsEmpty());
    ASSERT(!resTypeId.IsEmpty());

    Resources *rs = Resources::GetActive();
    if (!Resources::Contains(resTypeId, guid))
    {
        Resources::Add(resTypeId, resource);
    }
    ++rs->m_resourcesCache.Get(resTypeId).Get(guid).usageCount;
}

void Resources::UnRegisterResourceUsage(const TypeId &resTypeId,
                                        Resource *resource)
{
    const GUID &guid = resource->GetGUID();
    ASSERT(!guid.IsEmpty());
    ASSERT(!resTypeId.IsEmpty());

    Resources *rs = Resources::GetActive();
    if (rs)
    {
        ASSERT(Resources::Contains(resTypeId, guid));
        uint *resourcesUsage = &(rs->m_resourcesCache.Get(resTypeId)
                                 .Get(guid).usageCount);
        ASSERT(*resourcesUsage >= 1);
        --(*resourcesUsage);

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

void Resources::Destroy(Resource *resource)
{
    Asset *asset = DCAST<Asset*>(resource);
    if (asset) { Asset::Destroy(asset); } else { delete resource; }
}

Array<Resource *> Resources::GetCached(const GUID &guid)
{
    Resources *rs = Resources::GetActive();

    Array<Resource*> foundResources;
    for (const auto &map : rs->m_resourcesCache)
    {
        if ( map.second.ContainsKey(guid) )
        {
            foundResources.PushBack(map.second.Get(guid).resource);
        }
    }
    return foundResources;
}

Array<Resource *> Resources::GetCached(const Path &path)
{
    GUID guid = ImportFilesManager::GetGUIDFromFilepath(path);
    return GetCached(guid);
}

Resource *Resources::GetCached(const TypeId &resTypeId, const GUID &guid)
{
    Resources *rs = Resources::GetActive();
    if (!rs->m_resourcesCache.ContainsKey(resTypeId)) { return nullptr; }
    if (!rs->m_resourcesCache.Get(resTypeId).ContainsKey(guid)) { return nullptr; }
    return rs->m_resourcesCache.Get(resTypeId).Get(guid).resource;
}

Path Resources::GetResourcePath(Resource *resource)
{
    Path resPath = ImportFilesManager::GetFilepath(resource->GetGUID().
                                                   WithoutInsideFileGUID());
    const GUID::GUIDType insideFileGUID = resource->GetGUID().GetInsideFileGUID();
    if (insideFileGUID != 0)
    {
        String insideFileResourceName =
                    resource->GetInsideFileResourceName(insideFileGUID);
        resPath = resPath.Append(insideFileResourceName);
    }
    return resPath;
}

String Resources::ToString(Resource *resource)
{
    String result = "";
    Resources *rs = Resources::GetActive();
    for (const auto &pair : rs->m_resourcesCache)
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

MeshFactory *Resources::CreateMeshFactory() const
{
    return new MeshFactory();
}
