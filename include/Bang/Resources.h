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

FORWARD class Asset;
FORWARD class MeshFactory;
FORWARD class MaterialFactory;
FORWARD class ShaderProgramFactory;

class Resources
{
public:
    Resources();
    virtual ~Resources();

    template <class IResourceClass>
    static RH<IResourceClass> Load(const Path &filepath);

    template <class IResourceClass>
    static RH<IResourceClass> Load(const String &filepath);

    template <class IResourceClass>
    static RH<IResourceClass> Load(const GUID &guid);

    template<class IResourceClass, class ...Args>
    static RH<IResourceClass> Create(const Args&... args);

    template<class IResourceClass>
    static RH<IResourceClass> Clone(const RH<IResourceClass> &src);

    template <class IResourceClass>
    static Array<IResourceClass*> GetAll();
    static Array<IResource*> GetAllResources();

    #ifdef DEBUG
    static bool AssertCreatedFromResources();
    static bool AssertDestroyedFromResources();
    #endif

private:
public:
    #ifdef DEBUG
    static bool _AssertCreatedFromResources;
    static bool _AssertDestroyedFromResources;
    #endif

    struct ResourceEntry : public IToString
    {
        IResource *resource = nullptr;
        uint usageCount = 0; // Number of RH's using this resource entry
        String ToString() const override
        {
            return "RE(" + String(resource) + ", " + String(usageCount) + ")";
        }
    };
    TypeMap< Map<GUID, ResourceEntry> > m_GUIDCache;

    MeshFactory *m_meshFactory = nullptr;
    MaterialFactory *m_materialFactory = nullptr;
    ShaderProgramFactory *m_shaderProgramFactory = nullptr;

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

    static void Destroy(IResource *resource);

    template<class IResourceClass>
    static IResourceClass* GetCached(const GUID &guid);
    static IResource* GetCached(const TypeId &resTypeId, const GUID &guid);

    void Destroy();

    static Resources* GetActive();

    friend class Window;
    friend class MeshFactory;
    friend class IResourceHandle;
    friend class MaterialFactory;
    friend class ShaderProgramFactory;
};

NAMESPACE_BANG_END

#include "Resources.tcc"

#endif // RESOURCES_H
