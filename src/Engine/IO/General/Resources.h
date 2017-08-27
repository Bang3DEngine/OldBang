#ifndef RESOURCES_H
#define RESOURCES_H

#include <sstream>

#include "Bang/Map.h"
#include "Bang/Paths.h"
#include "Bang/Resource.h"
#include "Bang/XMLParser.h"
#include "Bang/ImportFilesManager.h"
#include "Bang/SerializableObject.h"

class Resources
{
public:
    Resources();
    virtual ~Resources();

    static void Add(const GUID& id, Resource *x);

    template <class ResourceSubClass>
    static TT_SUBCLASS(ResourceSubClass, Resource)*
    Load(const Path &filepath)
    {
        if (!filepath.IsFile()) { return nullptr; }

        Resource *res = Resources::GetCached<ResourceSubClass>(
                    ImportFilesManager::GetGUIDFromFilepath(filepath) );
        Debug_Log("Load filepath: " << filepath << ", res: " << res);
        if (!res)
        {
            res = new ResourceSubClass();
            res->Import(filepath);

            Path importFilepath = ImportFilesManager::GetImportFilePath(filepath);
            res->ReadFromFile(importFilepath);
            if (res->GetGUID().IsEmpty())
            {
                res->SetGUID( GUIDManager::GetNewGUID() );
            }
            Resources::Add(res->GetGUID(), res);
        }
        Debug_Log("Returning resource " << filepath << ": " << res->GetGUID());
        return DCAST<ResourceSubClass*>(res);
    }

    template <class T>
    static T* Load(const String &filepath) { return Load<T>(PPATH(filepath)); }

    template <class T>
    static T* Load(const GUID &guid)
    {
        if (guid.IsEmpty()) { return nullptr; }

        if (!Resources::Contains(guid))
        {
            Debug_Log(guid << ": " << ImportFilesManager::GetFilepath(guid));
            Resources::Load<T>( ImportFilesManager::GetFilepath(guid) );
        }
        return Resources::GetCached<T>(guid);
    }

    template <class T>
    static Array<T*> GetAll()
    {
        Array<T*> result;
        Resources *rs =  Resources::GetInstance();
        for (auto &itPair : rs->m_GUIDToResource)
        {
            T *resourceT = DCAST<T*>(itPair.second);
            if (resourceT) { result.PushBack(resourceT); }
        }
        return result;
    }

    static void UnLoad(const GUID &guid, bool deleteResource = false);
    static void UnLoad(Resource *res, bool deleteResource = false);

private:
    Map<GUID, Resource*> m_GUIDToResource;

    static bool Contains(const GUID &guid);

    template<class T>
    static T* GetCached(const GUID &guid)
    {
        Resource *res = GetCached(guid);
        return res ? DCAST<T*>(res) : nullptr;
    }

    static Resource* GetCached(const GUID &guid);
    static Resources* GetInstance();
};

#endif // RESOURCES_H
