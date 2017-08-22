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
        const GUID &guid = ImportFilesManager::GetGUIDFromFilepath(filepath);
        if (!Resources::Contains(guid))
        {
            Resource *res = new ResourceSubClass();
            Path importFile = ImportFilesManager::GetImportFilePath(filepath);
            res->Read( XMLParser::FromFile(importFile) );
            Resources::Add(guid, res);
        }
        return Resources::GetCached<ResourceSubClass>(guid);
    }

    template <class T>
    static T* Load(const String &filepath) { return Load<T>(PPATH(filepath)); }

    template <class T>
    static T* Load(const GUID &guid)
    {
        if (!Resources::Contains(guid))
        {
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
