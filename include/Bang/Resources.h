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
#include "Bang/ResourceHandle.h"
#include "Bang/ImportFilesManager.h"

NAMESPACE_BANG_BEGIN

FORWARD   class Asset;
class Resources
{
public:
    Resources();
    virtual ~Resources();

    template <class IResourceClass>
    static void Load(RH<IResourceClass> *handle, const Path &filepath);

    template <class IResourceClass>
    static void Load(RH<IResourceClass> *handle, const String &filepath);

    template <class IResourceClass>
    static void Load(RH<IResourceClass> *handle, const GUID &guid);

    template<class IResourceClass, class ...Args>
    static void Create(RH<IResourceClass> *handle, const Args&... args);

    template<class IResourceClass>
    static void Clone(const RH<IResourceClass> &src, RH<IResourceClass> *dst);

    template <class IResourceClass>
    static Array<IResourceClass*> GetAll();
    static Array<IResource*> GetAllResources();

    #ifdef DEBUG
    static bool AssertCreatedFromResources();
    static bool AssertDestroyedFromResources();
    #endif

private:
    #ifdef DEBUG
    static bool _AssertCreatedFromResources;
    static bool _AssertDestroyedFromResources;
    #endif

    struct ResourceEntry
    {
        IResource *resource = nullptr;
        uint usageCount = 0; // Number of RH's using this resource entry
    };
    TypeMap< Map<GUID, ResourceEntry> > m_GUIDCache;

    static void Add(const TypeId &resTypeId, IResource *res);

    static void RegisterResourceUsage(const TypeId &resTypeId, IResource *resource);
    static void UnRegisterResourceUsage(const TypeId &resTypeId, IResource *resource);

    static void Remove(const TypeId &resTypeId, const GUID &guid);

    template<class IResourceClass, class ...Args>
    static IResourceClass* _Create(const Args&... args);

    template<class IResourceClass, class ...Args>
    static typename std::enable_if<T_SUBCLASS(IResourceClass, Asset),
           IResourceClass*>::type _JustCreate(const Args&... args);

    template<class IResourceClass, class ...Args>
    static typename std::enable_if<T_NOT_SUBCLASS(IResourceClass, Asset),
           IResourceClass*>::type _JustCreate(const Args&... args);

    template<class IResourceClass>
    static bool Contains(const GUID &guid);
    static bool Contains(const TypeId &resourceClassTypeId, const GUID &guid);
    static bool ContainsGUID(const GUID &guid);

    static void Destroy(IResource *resource);

    template<class IResourceClass>
    static IResourceClass* GetCached(const GUID &guid);
    static IResource* GetCached(const TypeId &resTypeId, const GUID &guid);

    static Resources* GetActive();

    friend class IResourceHandle;
};

NAMESPACE_BANG_END

#include "Resources.tcc"

#endif // RESOURCES_H
