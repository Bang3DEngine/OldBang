#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include <sstream>

#include "Bang/Map.h"
#include "Bang/Debug.h"
#include "Bang/Asset.h"
#include "Bang/Object.h"
#include "Bang/XMLParser.h"
#include "Bang/IO.h"
#include "Bang/BehaviourManager.h"

class AssetsManager
{
public:

    /** For every id, it contains a pointer to the asset
      * created when the assets were loaded.
    **/
    Map<String, Asset*> m_idToAssetPointer;

    AssetsManager();
    virtual ~AssetsManager();

    static bool IsLoaded(const String &filepath, bool isEngineAsset = false);

    /** This function prepares a relative path, to be searched by
     *   a loading function. Or to be saved to the AssetsManager cache.
    **/
    static String FormatFilepath(const String &filepath, bool isEngineFilepath);

    template <class T>
    static T* GetAsset(const String &filepath, bool isEngineAsset)
    {
        return Object::Cast<T>( GetAsset(filepath, isEngineAsset) );
    }

    static Asset* GetAsset(const String &filepath, bool isEngineAsset);


    /** Reads a specific asset file (btex2d, bmesh, etc.)
      * from a filepath.
    **/
    template <class T>
    static T* ReadAssetFile(const String &filepath, bool isEngineAsset)
    {
        if (filepath.Empty()) { return nullptr; }

        T *a = nullptr;
        String absPath = AssetsManager::FormatFilepath(filepath, isEngineAsset);
        XMLNode xmlInfo = XMLParser::FromFile(absPath);
        a = new T();
        a->Read(xmlInfo);
        a->m_assetFilepath = absPath;
        return a;
    }

    /** Tries to retrieve an Asset from the AssetsManager cache
      * If there's no such Asset, it is loaded, added to the cache, and returned
      * If the file can't be read, nullptr is returned  **/
    template <class T>
    static T* Create(const String &filepath,
                     bool isEngineAsset = false)
    {
        T *a = new T();
        a->m_assetFilepath = filepath;
        AssetsManager::SaveAssetToFile(filepath, a, isEngineAsset);
        return a;
    }

    /** Tries to retrieve an Asset from the AssetsManager cache
      * If there's no such Asset, it is loaded, added to the cache, and returned
      * If the file can't be read, nullptr is returned  **/
    template <class T>
    static T* Load(const String &filepath, bool isEngineAsset = false)
    {
        if (filepath.Empty()) { return nullptr; }

        T *a = nullptr;
        if (!AssetsManager::IsLoaded(filepath, isEngineAsset))
        {
            a = AssetsManager::ReadAssetFile<T>(filepath, isEngineAsset);
            AssetsManager::SaveAssetToMap(filepath, a, isEngineAsset);
        }
        else
        {
            a = AssetsManager::GetAsset<T>(filepath, isEngineAsset);
        }
        return a;
    }

    #ifdef BANG_EDITOR
    static void UpdateAsset(const String &assetFilepath,
                            const XMLNode &xmlChangedInfo);
    static void InvalidateAsset(const String &assetFilepath);
    static void ReloadAsset(const String &assetFilepath);
    #endif

    static void SaveAssetToMap(const String &filepath, Asset* asset,
                               bool isEngineAsset);
    static void SaveAssetToFile(const String &filepath, Asset* asset,
                                bool isEngineAsset);

    static void Unload(const String &id);
    static void Unload(Asset *asset);

    static AssetsManager* GetCurrent();

    friend class Asset;
};

#endif // ASSETSMANAGER_H
