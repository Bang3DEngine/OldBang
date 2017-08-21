#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include <sstream>

#include "Bang/Map.h"
#include "Bang/Asset.h"
#include "Bang/Paths.h"
#include "Bang/Resource.h"
#include "Bang/XMLParser.h"
#include "Bang/SerializableObject.h"

class Resources
{
public:
    Resources();
    virtual ~Resources();

    static void Add(const String& id, Resource *x);

    template <class T>
    static
    typename std::enable_if<!T_SUBCLASS(T, Asset) &&
                             T_SUBCLASS(T, Resource) &&
                             T_SUBCLASS(T, SerializableObject), T>::type*
    Load(const Path &filepath)
    {
        return Resources::_Load<T>(filepath);
    }
    // Loads Non-Asset resources

    template <class AssetClass>
    static TT_SUBCLASS(AssetClass, Asset)* Load(const Path &filepath)
    {
        AssetClass *a = _Load<AssetClass>(filepath);
        if (a) { a->m_assetFilepath = filepath; }
        return a;
    }
    // Loads Asset resources

    template <class T>
    static T* Load(const String &filepath) { return Load<T>(PPATH(filepath)); }

    template <class T>
    static T* Get(const String &id) { return DCAST<T*>( Get(id) ); }

    template <class T>
    static Array<T*> GetAll()
    {
        Array<T*> result;
        Resources *rs =  Resources::GetInstance();
        for (auto &itPair : rs->m_idToResource)
        {
            T *resourceT = DCAST<T*>(itPair.second);
            if (resourceT) { result.PushBack(resourceT); }
        }
        return result;
    }

    static void UnLoad(const String &id, bool deleteResource = false);
    static void UnLoad(Resource *res, bool deleteResource = false);

private:
    Map<String, Resource*> m_idToResource;

    template <class T>
    static T* _Load(const Path &filepath)
    {
        const String id = filepath.GetAbsolute();
        if (!Resources::Contains(id))
        {
            T *res = new T();
            res->Read( XMLParser::FromFile(filepath) );
            Resources::Add(id, SCAST<Resource*>(res));
        }
        return Resources::Get<T>(id);
    }

    static bool Contains(const String &id);
    static Resource* Get(const String &id);
    static Resources* GetInstance();
};

#endif // ASSETSMANAGER_H
