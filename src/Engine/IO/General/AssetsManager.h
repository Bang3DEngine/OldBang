#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include <sstream>

#include "Bang/Map.h"
#include "Bang/Asset.h"
#include "Bang/Paths.h"
#include "Bang/Object.h"
#include "Bang/XMLParser.h"

class AssetsManager
{
public:

    /** For every id, it contains a pointer to the asset
      * created when the assets were loaded.
    **/
    Map<Path, Asset*> m_pathsToAssets;

    AssetsManager();
    virtual ~AssetsManager();

    static bool IsLoaded(const Path &filepath);

    template <class T>
    static T* GetAsset(const Path &filepath)
    {
        return DCAST<T*>( GetAsset(filepath) );
    }

    static Asset* GetAsset(const Path &filepath);


    /** Reads a specific asset file (btex2d, bmesh, etc.)
      * from a filepath.
    **/
    template <class T>
    static T* ReadAssetFile(const Path &filepath)
    {
        if (filepath.IsEmpty()) { return nullptr; }

        T *a = nullptr;
        XMLNode xmlInfo = XMLParser::FromFile(filepath);
        a = new T();
        a->Read(xmlInfo);
        a->m_assetFilepath = filepath;
        return a;
    }

    /** Tries to retrieve an Asset from the AssetsManager cache
      * If there's no such Asset, it is loaded, added to the cache, and returned
      * If the file can't be read, nullptr is returned  **/
    template <class T>
    static T* Create(const Path &filepath)
    {
        T *a = new T();
        a->m_assetFilepath = filepath;
        AssetsManager::SaveAssetToFile(filepath, a);
        return a;
    }

    /** Tries to retrieve an Asset from the AssetsManager cache
      * If there's no such Asset, it is loaded, added to the cache, and returned
      * If the file can't be read, nullptr is returned  **/
    template <class T>
    static T* Load(const Path &filepath)
    {
        if (filepath.IsEmpty()) { return nullptr; }
        if (!filepath.Exists() || !filepath.IsFile()) { return nullptr; }

        T *a = nullptr;
        if (!AssetsManager::IsLoaded(filepath))
        {
            a = AssetsManager::ReadAssetFile<T>(filepath);
            AssetsManager::SaveAssetToMap(filepath, a);
        }
        else
        {
            a = AssetsManager::GetAsset<T>(filepath);
        }
        return a;
    }
    template <class T>
    static T* Load(const String &filepath)
    {
        return AssetsManager::Load<T>( PPATH(filepath) );
    }

    static void UpdateAsset(const Path &assetFilepath,
                            const XMLNode &xmlChangedInfo);
    static void InvalidateAsset(const Path &assetFilepath);
    static void ReloadAsset(const Path &assetFilepath);

    static void SaveAssetToMap(const Path &filepath, Asset* asset);
    static void SaveAssetToFile(const Path &filepath, Asset* asset);

    static void Unload(const Path &filepath);
    static void Unload(Asset *asset);

    static AssetsManager* GetCurrent();

    friend class Asset;
};

#endif // ASSETSMANAGER_H
