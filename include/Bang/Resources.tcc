#include "Bang/Resources.h"

#include "Bang/ObjectManager.h"

NAMESPACE_BANG_BEGIN

template <class IResourceClass>
void Resources::Load(RH<IResourceClass> *handle, const Path &filepath)
{
    if (!filepath.IsFile())
    {
        Debug_Error("Trying to load file '" << filepath <<
                    "' which does not exist.");
        handle->Set(nullptr);
        return;
    }

    IResourceClass* res = Resources::GetCached<IResourceClass>(
                            ImportFilesManager::GetGUIDFromFilepath(filepath) );
    if (!res)
    {
        res = Resources::_Create<IResourceClass>();
        res->Import(filepath);

        Path importFilepath = ImportFilesManager::GetImportFilePath(filepath);
        res->ImportXMLFromFile(importFilepath);
    }

    handle->Set( res );
}

template <class IResourceClass>
void Resources::Load(RH<IResourceClass> *handle, const String &filepath)
{
    Resources::Load<IResourceClass>(handle, PPATH(filepath));
}

template <class IResourceClass>
void Resources::Load(RH<IResourceClass> *handle, const GUID &guid)
{
    if (!guid.IsEmpty())
    {
        if (!Resources::Contains<IResourceClass>(guid))
        {
            Path filepath = ImportFilesManager::GetFilepath(guid);
            Debug_Log("Going to load from filepath with GUID " << guid);
            Resources::Load<IResourceClass>(handle, filepath);
        }
        else
        {
            handle->Set( Resources::GetCached<IResourceClass>(guid) );
        }
    }
    else
    {
        handle->Set(nullptr);
    }
}

template<class IResourceClass, class ...Args>
void Resources::Create(RH<IResourceClass> *handle, const Args&... args)
{
    handle->Set( Resources::_Create<IResourceClass>(args...) );
}

template<class IResourceClass, class ...Args>
IResourceClass* Resources::_Create(const Args&... args)
{
    #ifdef DEBUG
    Resources::_AssertCreatedFromResources = true;
    #endif

    IResourceClass *res = Resources::_JustCreate<IResourceClass>(args...);
    if (res->GetGUID().IsEmpty())
    {
        res->SetGUID( GUIDManager::GetNewGUID() );
        Debug_Log("Empty GUID, generating to " << res->GetGUID());
    }

    #ifdef DEBUG
    Resources::_AssertCreatedFromResources = false;
    #endif

    return res;
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
void Resources::Clone(const RH<IResourceClass> &src, RH<IResourceClass> *dst)
{
    ASSERT(&src != dst);
    Resources::Create<IResourceClass>(dst);
    if (src)
    {
        src.Get()->CloneInto( dst->Get() );
    }
}

NAMESPACE_BANG_END
