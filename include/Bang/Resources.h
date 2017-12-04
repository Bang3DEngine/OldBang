#ifndef RESOURCES_H
#define RESOURCES_H

#include <sstream>

#include "Bang/Map.h"
#include "Bang/Asset.h"
#include "Bang/Paths.h"
#include "Bang/TypeMap.h"
#include "Bang/Resource.h"
#include "Bang/XMLParser.h"
#include "Bang/Serializable.h"
#include "Bang/ObjectManager.h"
#include "Bang/ImportFilesManager.h"

NAMESPACE_BANG_BEGIN

FORWARD class Asset;

class Resources
{
public:
    Resources();
    virtual ~Resources();

    template <class IResourceClass>
    static void Add(const GUID& guid, IResource *x);

    template <class IResourceClass>
    static TT_SUBCLASS(IResourceClass, IResource)* Load(const Path &filepath);

    template <class IResourceClass>
    static TT_SUBCLASS(IResourceClass, IResource)* Load(const String &filepath);

    template <class IResourceClass>
    static TT_SUBCLASS(IResourceClass, IResource)* Load(const GUID &guid);

    template<class IResourceClass, class ...Args>
    static IResourceClass* Create(const Args&... args);

    template <class IResourceClass>
    static Array<IResourceClass*> GetAll();
    static Array<IResource*> GetAllResources();

    static void Unload(const GUID &guid);
    static void Unload(IResource *res);
    static void UnloadSingleResource(IResource *res);

    #ifdef DEBUG
    static bool AssertCreatedFromResources();
    static bool AssertDestroyedFromResources();
    #endif

private:
    #ifdef DEBUG
    static bool _AssertCreatedFromResources;
    static bool _AssertDestroyedFromResources;
    #endif

    Map<IResource*, uint> m_resourcesUsage;
    TypeMap< Map<GUID, IResource*> > m_GUIDToResource;

    template<class IResourceClass, class ...Args>
    static typename std::enable_if<T_SUBCLASS(IResourceClass, Asset),
                    IResourceClass*>::type JustCreate(const Args&... args);

    template<class IResourceClass, class ...Args>
    static typename std::enable_if<T_NOT_SUBCLASS(IResourceClass, Asset),
                    IResourceClass*>::type JustCreate(const Args&... args);

    template<class IResourceClass>
    static bool Contains(const GUID &guid);

    static void Destroy(IResource *resource);

    template<class IResourceClass>
    static IResourceClass* GetCached(const GUID &guid);

    static Resources* GetInstance();
};

NAMESPACE_BANG_END

#include "Resources.tcc"

#endif // RESOURCES_H
