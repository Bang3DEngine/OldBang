#ifndef RESOURCES_H
#define RESOURCES_H

#include <sstream>

#include "Bang/Map.h"
#include "Bang/Paths.h"
#include "Bang/TypeMap.h"
#include "Bang/Resource.h"
#include "Bang/XMLParser.h"
#include "Bang/ImportFilesManager.h"
#include "Bang/SerializableObject.h"

class Resources
{
public:
    Resources();
    virtual ~Resources();

    template <class ResourceClass>
    static void Add(const GUID& guid, Resource *x);

    template <class ResourceClass>
    static TT_SUBCLASS(ResourceClass, Resource)* Load(const Path &filepath);

    template <class ResourceClass>
    static ResourceClass* Load(const String &filepath);

    template <class ResourceClass>
    static ResourceClass* Load(const GUID &guid);

    template <class ResourceClass>
    static Array<ResourceClass*> GetAll();
    static Array<Resource*> GetAllResources();

    static void UnLoad(const GUID &guid, bool deleteResource = false);
    static void UnLoad(Resource *res, bool deleteResource = false);

private:
    using GUIDToResourceMap = Map<GUID, Resource*>;
    TypeMap<GUIDToResourceMap> m_GUIDToResource;

    template<class ResourceClass>
    static bool Contains(const GUID &guid);

    template<class ResourceClass>
    static ResourceClass* GetCached(const GUID &guid);

    static Resources* GetInstance();
};

#include "Resources.tcc"

#endif // RESOURCES_H
