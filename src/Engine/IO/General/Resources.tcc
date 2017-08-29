#include "Bang/Resources.h"

template<class ResourceClass>
void Resources::Add(const GUID& guid, Resource *x)
{
    ENSURE(x && !guid.IsEmpty());
    Resources *rs = Resources::GetInstance();
    if (!rs->m_GUIDToResource.ContainsKey<ResourceClass>())
    {
        rs->m_GUIDToResource.Add<ResourceClass>();
    }
    rs->m_GUIDToResource.Get<ResourceClass>().Add(guid, x);
}

template <class ResourceClass>
TT_SUBCLASS(ResourceClass, Resource)* Resources::Load(const Path &filepath)
{
    if (!filepath.IsFile()) { return nullptr; }

    Resource *res = Resources::GetCached<ResourceClass>(
                ImportFilesManager::GetGUIDFromFilepath(filepath) );
    if (!res)
    {
        res = new ResourceClass();
        res->Import(filepath);

        Path importFilepath = ImportFilesManager::GetImportFilePath(filepath);
        res->ReadFromFile(importFilepath);
        if (res->GetGUID().IsEmpty())
        {
            res->SetGUID( GUIDManager::GetNewGUID() );
        }
        Resources::Add<ResourceClass>(res->GetGUID(), res);
    }
    return DCAST<ResourceClass*>(res);
}

template <class ResourceClass>
ResourceClass* Resources::Load(const String &filepath)
{
    return Load<ResourceClass>(PPATH(filepath));
}

template <class ResourceClass>
ResourceClass* Resources::Load(const GUID &guid)
{
    if (guid.IsEmpty()) { return nullptr; }

    if (!Resources::Contains<ResourceClass>(guid))
    {
        Resources::Load<ResourceClass>( ImportFilesManager::GetFilepath(guid) );
    }
    return Resources::GetCached<ResourceClass>(guid);
}

template <class ResourceClass>
Array<ResourceClass*> Resources::GetAll()
{
    Array<ResourceClass*> result;
    Array<Resource*> resources = Resources::GetAllResources();
    for (Resource *res : resources)
    {
        ResourceClass *rc = DCAST<ResourceClass*>(res);
        if (rc) { result.PushBack(rc); }
    }
    return result;
}

template<class ResourceClass>
bool Resources::Contains(const GUID &guid)
{
    if (guid.IsEmpty()) { return false; }
    return Resources::GetCached<ResourceClass>(guid) != nullptr;
}

template<class ResourceClass>
ResourceClass* Resources::GetCached(const GUID &guid)
{
    Resources *rs = Resources::GetInstance();
    if (!rs->m_GUIDToResource.ContainsKey<ResourceClass>()) { return nullptr; }
    Resource *res = rs->m_GUIDToResource.Get<ResourceClass>().Get(guid);
    return res ? DCAST<ResourceClass*>(res) : nullptr;
}
