#include "Bang/Resources.h"

#include "Bang/ObjectManager.h"

NAMESPACE_BANG_BEGIN

template <class IResourceClass>
RH<IResourceClass> Resources::Load(const Path &filepath)
{
    IResourceClass* res = Resources::GetCached<IResourceClass>(
                            ImportFilesManager::GetGUIDFromFilepath(filepath) );
    if (!res)
    {
        res = Resources::_Create<IResourceClass>();

        Path importFilepath = ImportFilesManager::GetImportFilepath(filepath);

        // Import XML before to get resource GUID at least
        res->ImportXMLFromFile(importFilepath);
        res->Import(filepath); // Import now from filepath itself
        res->ImportXMLFromFile(importFilepath); // Import XML after again
    }

    return RH<IResourceClass>(res);
}

template <class IResourceClass>
RH<IResourceClass> Resources::Load(const String &filepath)
{
    return Resources::Load<IResourceClass>( EPATH(filepath) );
}

template <class IResourceClass>
RH<IResourceClass> Resources::Load(const GUID &guid)
{
    if (!guid.IsEmpty())
    {
        const GUID::GUIDType insideFileGUID = guid.GetInsideFileGUID();
        if (insideFileGUID == 0) // Normal resource (file attached to it)
        {
            if (!Resources::Contains<IResourceClass>(guid))
            {
                // If we don't have it loaded, load/create a new one
                Path filepath = ImportFilesManager::GetFilepath(guid);
                if (filepath.IsFile())
                {
                    return Resources::Load<IResourceClass>(filepath);
                }
                else
                {
                    RH<IResourceClass> rh = Resources::Create<IResourceClass>(guid);
                    return rh;
                }
            }
            else
            {
                // Load it from cache
                return RH<IResourceClass>(
                            Resources::GetCached<IResourceClass>(guid) );
            }
        }
        else  // Inner resource (resource embedded into another resource)
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
                    // the handler, and return it 3
                    Resource *innerRes =
                        parentRes.Get()->GetInsideFileResource(insideFileGUID);
                    RH<IResourceClass> innerResRH;
                    if (innerRes)
                    {
                        innerResRH.Set( DCAST<IResourceClass*>(innerRes) );
                    }
                    return innerResRH;
                }
            }
        }
    }

    return RH<IResourceClass>();
}

template<class IResourceClass, class ...Args>
RH<IResourceClass> Resources::Create(const Args&... args)
{
    return RH<IResourceClass>( Resources::_Create<IResourceClass>(args...) );
}
template<class IResourceClass, class ...Args>
RH<IResourceClass> Resources::Create(const GUID &guid, const Args&... args)
{
    return RH<IResourceClass>( Resources::_Create<IResourceClass>(guid, args...) );
}

template<class IResourceClass, class ...Args>
RH<IResourceClass> Resources::CreateInnerResource(const GUID &baseGUID,
                                                  const GUID::GUIDType insideFileGUID,
                                                  const Args&... args)
{
    GUID resourceInsideFileGUID;
    GUIDManager::CreateInsideFileGUID(baseGUID, insideFileGUID,
                                      &resourceInsideFileGUID);
    return Resources::Create<IResourceClass, Args...>(resourceInsideFileGUID,
                                                      args...);
}

template<class IResourceClass, class ...Args>
IResourceClass* Resources::_Create(const GUID &guid, const Args&... args)
{
    #ifdef DEBUG
    Resources::_AssertCreatedFromResources = true;
    #endif

    IResourceClass *res = Resources::_JustCreate<IResourceClass>(args...);
    res->SetGUID(guid);

    #ifdef DEBUG
    Resources::_AssertCreatedFromResources = false;
    #endif

    return res;
}
template<class IResourceClass, class ...Args>
IResourceClass* Resources::_Create(const Args&... args)
{
    GUID guid = GUIDManager::GetNewGUID();
    return Resources::_Create<IResourceClass, Args...>( guid, args... );
}

template<class IResourceClass, class ...Args>
typename std::enable_if<T_SUBCLASS(IResourceClass, Asset),
         IResourceClass*>::type Resources::_JustCreate(const Args&... args)
{
    return Asset::Create<IResourceClass>(args...);
}

template<class IResourceClass, class ...Args>
typename std::enable_if<T_NOT_SUBCLASS(IResourceClass, Asset),
         IResourceClass*>::type  Resources::_JustCreate(const Args&... args)
{
    return new IResourceClass(args...);
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
    return Resources::Contains(GetTypeId<IResourceClass>(), guid);
}

template<class IResourceClass>
IResourceClass* Resources::GetCached(const GUID &guid)
{
    return SCAST<IResourceClass*>(
                Resources::GetCached(GetTypeId<IResourceClass>(), guid) );
}

template<class IResourceClass>
RH<IResourceClass> Resources::Clone(const RH<IResourceClass> &src)
{
    RH<IResourceClass> rh;
    if (src.Get())
    {
        rh = Resources::Create<IResourceClass>();
        src.Get()->CloneInto( rh.Get() );
    }
    return rh;
}

NAMESPACE_BANG_END
