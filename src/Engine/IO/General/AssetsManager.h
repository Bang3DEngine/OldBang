#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include <sstream>

#include "Bang/Map.h"
#include "Bang/Asset.h"
#include "Bang/Paths.h"
#include "Bang/XMLParser.h"

class AssetsManager
{
public:
    AssetsManager();
    virtual ~AssetsManager();

    template <class T>
    static void Add(const String& id, T& x)
    {
        AssetsManager *am = AssetsManager::GetInstance();
        if (!id.IsEmpty()) { am->m_idToAsset.Add(id, x); }
    }

    template <class T>
    static T* Load(const Path &filepath)
    {
        const String id = filepath.GetAbsolute();
        if (!AssetsManager::Contains(id))
        {
            T *a = new T();
            a->Read( XMLParser::FromFile(filepath) );
            a->m_assetFilepath = filepath;
            AssetsManager::Add(id, a);
        }
        return AssetsManager::Get<T>(id);
    }
    template <class T>
    static T* Load(const String &filepath)
    {
        return Load<T>( PPATH(filepath) );
    }

    template <class T>
    static T* Get(const String &id)
    {
        return DCAST<T*>( Get(id) );
    }

    static void Unload(const String &id);
    static void Unload(Asset *asset);

private:
    Map<String, Asset*> m_idToAsset;

    static bool Contains(const String &id);
    static Asset* Get(const String &id);
    static AssetsManager* GetInstance();

    friend class Asset;
};

#endif // ASSETSMANAGER_H
