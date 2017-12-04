#include "Bang/Resources.h"

#include "Bang/ObjectManager.h"

NAMESPACE_BANG_BEGIN

template<class IResourceClass>
void Resources::Add(const GUID& guid, IResource *x)
{
    ENSURE(x && !guid.IsEmpty());
    Resources *rs = Resources::GetInstance();
    if (!rs->m_GUIDToResource.ContainsKey<IResourceClass>())
    {
        rs->m_GUIDToResource.Add<IResourceClass>();
    }
    rs->m_GUIDToResource.Get<IResourceClass>().Add(guid, x);
}

template <class IResourceClass>
TT_SUBCLASS(IResourceClass, IResource)* Resources::Load(const Path &filepath)
{
    if (!filepath.IsFile())
    {
        Debug_Error("Trying to load file '" << filepath <<
                    "' which does not exist.");
        return nullptr;
    }

    Resource *res = Resources::GetCached<IResourceClass>(
                ImportFilesManager::GetGUIDFromFilepath(filepath) );
    if (!res)
    {
        res = Create<IResourceClass>();
        res->Import(filepath);

        Path importFilepath = ImportFilesManager::GetImportFilePath(filepath);
        res->ImportXMLFromFile(importFilepath);
        if (res->GetGUID().IsEmpty())
        {
            res->SetGUID( GUIDManager::GetNewGUID() );
        }
        Resources::Add<IResourceClass>(res->GetGUID(), res);
    }
    return Cast<IResourceClass*>(res);
}

template <class ResourceClass>
ResourceClass* Resources::Load(const String &filepath)
{
    return Load<ResourceClass>(PPATH(filepath));
}

template <class IResourceClass>
IResourceClass* Resources::Load(const GUID &guid)
{
    if (guid.IsEmpty()) { return nullptr; }
    if (!Resources::Contains<IResourceClass>(guid))
    {
        Resources::Load<IResourceClass>( ImportFilesManager::GetFilepath(guid) );
    }
    return Resources::GetCached<IResourceClass>(guid);
}

template <class IResourceClass>
Array<IResourceClass*> Resources::GetAll()
{
    Array<IResourceClass*> result;
    Array<IResource*> resources = Resources::GetAllResources();
    for (IResource *res : resources)
    {
        IResourceClass *rc = Cast<IResourceClass*>(res);
        if (rc) { result.PushBack(rc); }
    }
    return result;
}

template<class IResourceClass>
bool Resources::Contains(const GUID &guid)
{
    if (guid.IsEmpty()) { return false; }
    return Resources::GetCached<IResourceClass>(guid) != nullptr;
}

template<class IResourceClass>
IResourceClass* Resources::GetCached(const GUID &guid)
{
    Resources *rs = Resources::GetInstance();
    if (!rs->m_GUIDToResource.ContainsKey<IResourceClass>()) { return nullptr; }
    IResource *res = rs->m_GUIDToResource.Get<IResourceClass>().Get(guid);
    return res ? Cast<IResourceClass*>(res) : nullptr;
}

NAMESPACE_BANG_END
