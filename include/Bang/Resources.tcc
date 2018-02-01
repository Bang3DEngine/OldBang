#include "Bang/Resources.h"

#include "Bang/Debug.h"

NAMESPACE_BANG_BEGIN

template <class ResourceClass>
RH<ResourceClass> Resources::Load(const Path &filepath)
{
    if (!filepath.Exists())
    {
        Debug_Warn("Filepath '" << filepath.GetAbsolute() << "' not found");
    }

    GUID resGUID = ImportFilesManager::GetGUIDFromFilepath(filepath);
    ResourceClass* res = Resources::GetCached<ResourceClass>(resGUID);
    if (!res)
    {
        res = Resources::_Create<ResourceClass>();

        Path importFilepath = ImportFilesManager::GetImportFilepath(filepath);

        // Import XML before to get resource GUID at least
        res->ImportXMLFromFile(importFilepath);
        res->Import(filepath); // Import now from filepath itself
        res->ImportXMLFromFile(importFilepath); // Import XML after again
    }

    return RH<ResourceClass>(res);
}

template <class ResourceClass>
RH<ResourceClass> Resources::Load(const String &filepathStr)
{
    Resources *rs = Resources::GetActive();
    Array<Path> lookupPaths = rs->GetLookUpPaths();

    Path filepath = Path::Empty;
    for (const Path &lookUpPath : lookupPaths)
    {
        filepath = lookUpPath.Append(filepathStr);
        if (filepath.IsFile()) { break; }
    }
    return Resources::Load<ResourceClass>(filepath);
}

template <class ResourceClass>
RH<ResourceClass> Resources::Load(const GUID &guid)
{
    if (!guid.IsEmpty())
    {
        const GUID::GUIDType insideFileGUID = guid.GetInsideFileGUID();
        if (insideFileGUID == 0) // Normal resource (file attached to it)
        {
            if (!Resources::Contains<ResourceClass>(guid))
            {
                // If we don't have it loaded, load/create a new one
                Path filepath = ImportFilesManager::GetFilepath(guid);
                if (filepath.IsFile())
                {
                    return Resources::Load<ResourceClass>(filepath);
                }
                else
                {
                    RH<ResourceClass> rh = Resources::Create<ResourceClass>(guid);
                    return rh;
                }
            }
            else
            {
                // Load it from cache
                return RH<ResourceClass>(
                            Resources::GetCached<ResourceClass>(guid) );
            }
        }
        else  // Inner resource (resource embedded into another resource)
        {
            if (!Resources::Contains<ResourceClass>(guid))
            {
                // It is a resource inside another resource. Find parent path,
                // load it, and retrieve from it the inner resource!
                GUID parentResourceGUID = guid.WithoutInsideFileGUID();
                Path parentFilepath = ImportFilesManager::GetFilepath(parentResourceGUID);
                if (parentFilepath.IsFile())
                {
                    // Load the parent resource guessing the type from the extension
                    RH<Resource> parentRes =
                                    Resources::LoadFromExtension(parentFilepath);
                    if (parentRes)
                    {
                        // Call virtual function that finds inner resource, create
                        // the handler, and return it
                        Resource *innerRes =
                            parentRes.Get()->GetInsideFileResource(insideFileGUID);
                        RH<ResourceClass> innerResRH;
                        if (innerRes)
                        {
                            innerResRH.Set( DCAST<ResourceClass*>(innerRes) );
                        }
                        return innerResRH;
                    }
                }
            }
            else
            {
                // Load it from cache
                return RH<ResourceClass>(
                            Resources::GetCached<ResourceClass>(guid) );
            }
        }
    }

    return RH<ResourceClass>();
}

template<class ResourceClass, class ...Args>
RH<ResourceClass> Resources::Create(const Args&... args)
{
    return RH<ResourceClass>( Resources::_Create<ResourceClass>(args...) );
}
template<class ResourceClass, class ...Args>
RH<ResourceClass> Resources::Create(const GUID &guid, const Args&... args)
{
    return RH<ResourceClass>( Resources::_Create<ResourceClass>(guid, args...) );
}

template<class ResourceClass, class ...Args>
RH<ResourceClass> Resources::CreateInnerResource(const GUID &baseGUID,
                                                 const GUID::GUIDType insideFileGUID,
                                                 const Args&... args)
{
    GUID resourceInsideFileGUID;
    GUIDManager::CreateInsideFileGUID(baseGUID, insideFileGUID,
                                      &resourceInsideFileGUID);
    return Resources::Create<ResourceClass, Args...>(resourceInsideFileGUID,
                                                      args...);
}

template<class ResourceClass, class ...Args>
ResourceClass* Resources::_Create(const GUID &guid, const Args&... args)
{
    ResourceClass *res = Resources::_JustCreate<ResourceClass>(args...);
    res->SetGUID(guid);

    return res;
}
template<class ResourceClass, class ...Args>
ResourceClass* Resources::_Create(const Args&... args)
{
    GUID guid = GUIDManager::GetNewGUID();
    return Resources::_Create<ResourceClass, Args...>( guid, args... );
}

template<class ResourceClass, class ...Args>
typename std::enable_if<T_SUBCLASS(ResourceClass, Asset),
         ResourceClass*>::type Resources::_JustCreate(const Args&... args)
{
    return Asset::Create<ResourceClass>(args...);
}

template<class ResourceClass, class ...Args>
typename std::enable_if<T_NOT_SUBCLASS(ResourceClass, Asset),
         ResourceClass*>::type  Resources::_JustCreate(const Args&... args)
{
    return new ResourceClass(args...);
}

template <class ResourceClass>
Array<ResourceClass*> Resources::GetAll()
{
    Array<ResourceClass*> result;
    Array<Resource*> resources = Resources::GetAllResources();
    for (Resource *res : resources)
    {
        ResourceClass *rc = Cast<ResourceClass*>(res);
        if (rc) { result.PushBack(rc); }
    }
    return result;
}

template<class ResourceClass>
bool Resources::Contains(const GUID &guid)
{
    return Resources::Contains(GetTypeId<ResourceClass>(), guid);
}

template<class ResourceClass>
ResourceClass* Resources::GetCached(const GUID &guid)
{
    return SCAST<ResourceClass*>(
                Resources::GetCached(GetTypeId<ResourceClass>(), guid) );
}

template<class ResourceClass>
RH<ResourceClass> Resources::Clone(const RH<ResourceClass> &src)
{
    RH<ResourceClass> rh;
    if (src.Get())
    {
        rh = Resources::Create<ResourceClass>();
        src.Get()->CloneInto( rh.Get() );
    }
    return rh;
}

NAMESPACE_BANG_END
