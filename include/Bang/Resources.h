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

    template <class ResourceClass>
    static RH<ResourceClass> Load(const Path &filepath);

    template <class ResourceClass>
    static RH<ResourceClass> Load(const String &filepath);

    template <class ResourceClass>
    static RH<ResourceClass> Load(const GUID &guid);

    static RH<Resource> LoadFromExtension(const Path &filepath);

    template<class ResourceClass, class ...Args>
    static RH<ResourceClass> Create(const Args&... args);
    template<class ResourceClass, class ...Args>
    static RH<ResourceClass> Create(const GUID &guid, const Args&... args);
    template<class ResourceClass, class ...Args>
    static RH<ResourceClass> CreateInnerResource(const GUID &baseGUID,
                                                  const GUID::GUIDType insideFileGUID,
                                                  const Args&... args);

    template<class ResourceClass>
    static RH<ResourceClass> Clone(const RH<ResourceClass> &src);

    template <class ResourceClass>
    static Array<ResourceClass*> GetAll();
    static Array<Resource*> GetAllResources();

    struct ResourceEntry : public IToString
    {
        Resource *resource = nullptr;
        uint usageCount = 0; // Number of RH's using this resource entry
        String ToString() const override
        {
            return "RE(" + String(resource) + ", " + String(usageCount) + ")";
        }
    };

    static void Add(const TypeId &resTypeId, Resource *res);

    static void SetPermanent(Resource *resource, bool permanent);
    static bool IsPermanent(Resource *resource);
    static void SetPermanent(const Path &resourcePath, bool permanent);
    static bool IsPermanent(const Path &resourcePath);

    static void RegisterResourceUsage(const TypeId &resTypeId, Resource *resource);
    static void UnRegisterResourceUsage(const TypeId &resTypeId, Resource *resource);

    static void Remove(const TypeId &resTypeId, const GUID &guid);

    template<class ResourceClass, class ...Args>
    static ResourceClass* _Create(const Args&... args);
    template<class ResourceClass, class ...Args>
    static ResourceClass* _Create(const GUID &guid, const Args&... args);

    template<class ResourceClass, class ...Args>
    static typename std::enable_if<T_SUBCLASS(ResourceClass, Asset),
           ResourceClass*>::type _JustCreate(const Args&... args);

    template<class ResourceClass, class ...Args>
    static typename std::enable_if<T_NOT_SUBCLASS(ResourceClass, Asset),
           ResourceClass*>::type _JustCreate(const Args&... args);

    template<class ResourceClass>
    static bool Contains(const GUID &guid);
    static bool Contains(const TypeId &resourceClassTypeId, const GUID &guid);

    static void Destroy(Resource *resource);

    template<class ResourceClass>
    static ResourceClass* GetCached(const GUID &guid);
    static Resource* GetCached(const TypeId &resTypeId, const GUID &guid);

    static Path GetResourcePath(Resource *resource);
    static String ToString(Resource *resource);
    static String ToString();

    void Destroy();

    static Resources* GetActive();

    #ifdef DEBUG
    static bool AssertCreatedFromResources();
    static bool AssertDestroyedFromResources();
    #endif

private:
    Set<Path> m_permanentResourcesPaths;
    Set<Resource*> m_permanentResources;
    TypeMap< Map<GUID, ResourceEntry> > m_GUIDCache;

    MeshFactory *m_meshFactory = nullptr;
    MaterialFactory *m_materialFactory = nullptr;
    ShaderProgramFactory *m_shaderProgramFactory = nullptr;

    #ifdef DEBUG
    static bool _AssertCreatedFromResources;
    static bool _AssertDestroyedFromResources;
    #endif

    friend class Window;
    friend class GUIDManager;
    friend class MeshFactory;
    friend class IResourceHandle;
    friend class MaterialFactory;
    friend class ShaderProgramFactory;
};

NAMESPACE_BANG_END

#include "Resources.tcc"

#endif // RESOURCES_H
