#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include <sstream>

#include "Bang/Map.h"
#include "Bang/Asset.h"
#include "Bang/Paths.h"
#include "Bang/Resource.h"
#include "Bang/XMLParser.h"

class Resources
{
public:
    Resources();
    virtual ~Resources();

    static void Add(const String& id, Resource *x);

    template <class T>
    static T* Load(const Path &filepath)
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

    template <class AssetClass, T_SUBCLASS(AssetClass, Asset)>
    static AssetClass* Load(const Path &filepath)
    {
        AssetClass *a = Load<AssetClass>(filepath);
        a->m_assetFilepath = filepath;
        return a;
    }

    template <class T>
    static T* Load(const String &filepath) { return Load<T>(PPATH(filepath)); }

    template <class T>
    static T* Get(const String &id) { return DCAST<T*>( Get(id) ); }

    static void Unload(const String &id);
    static void Unload(Resource *res);

private:
    Map<String, Resource*> m_idToResource;

    static bool Contains(const String &id);
    static Resource* Get(const String &id);
    static Resources* GetInstance();
};

#endif // ASSETSMANAGER_H
