#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include <sstream>

#include "Map.h"
#include "Debug.h"
#include "Asset.h"
#include "XMLParser.h"
#include "BehaviourManager.h"

class AssetsManager
{
private:

    /** For every id, it contains a pointer to the asset
      * created when the assets were loaded.
    **/
    Map<String, Asset*> m_id_To_AssetPointer;

    /** This function prepares a relative path, to be searched by a loading function.
     * This is, if you pass it "Images/wololo", it will prepend the Assets directory full path.
     * For example, it would return something like "/home/john123/MyProject/Assets/Images/wololo".
    **/
    static String FormatFilepathForSearchingFile(const String &filepath,
                                                 bool isEngineFilepath);

    /** This function prepares a path, to be saved as a map key in the assets cache.
     * It will make sure that the path is always the same (so an asset can be a cache hit even
     * if the passed path is not exactly the same).
     * The convention is to use as filepath key string something like: "./Images/wololo", where "."
     * is the Asset directory for the current project.
    **/
    static String FormatFilepathForCacheMap(const String &filepath,
                                            bool isEngineFilepath);

    template <class T>
    static T* GetAsset(const String &filepath, bool isEngineAsset)
    {
        AssetsManager *am = AssetsManager::GetCurrent();
        if (!AssetsManager::IsLoaded(filepath, isEngineAsset)) return nullptr;
        else
        {
            String f = AssetsManager::FormatFilepathForCacheMap(filepath,
                                                                isEngineAsset);
            return dynamic_cast<T*>(am->m_id_To_AssetPointer[f]);
        }
    }

    /** Reads a specific asset file (btex2d, bmesh, etc.)
      * from a filepath.
    **/
    template <class T>
    static T* ReadAssetFile(const String &filepath, bool isEngineAsset)
    {
        T *a = nullptr;
        String absPath = AssetsManager::FormatFilepathForSearchingFile(filepath,
                                                                       isEngineAsset);
        XMLNode *xmlInfo = XMLParser::FromFile(absPath);
        if (xmlInfo)
        {
            a = new T();
            a->ReadXMLInfo(xmlInfo);
            a->m_filepath = absPath;
            delete xmlInfo;
        }
        return a;
    }

    static void SaveAssetToMap(const String &filepath, Asset* asset, bool isEngineAsset);
    static void SaveAssetToFile(const String &filepath, Asset* asset, bool isEngineAsset);

    static AssetsManager* GetCurrent();

public:

    AssetsManager();
    virtual ~AssetsManager();

    static bool IsLoaded(const String &filepath,
                         bool isEngineAsset = false);

    /** Tries to retrieve an Asset from the AssetsManager cache
      * If there's no such Asset, it is loaded, added to the cache, and returned
      * If the file can't be read, nullptr is returned  **/
    template <class T>
    static T* Create(const String &filepath,
                     bool isEngineAsset = false)
    {
        T *a = new T(); //a->m_filepath = filepath;
        AssetsManager::SaveAssetToFile(filepath, a, isEngineAsset);
        return a;
    }

    /** Tries to retrieve an Asset from the AssetsManager cache
      * If there's no such Asset, it is loaded, added to the cache, and returned
      * If the file can't be read, nullptr is returned  **/
    template <class T>
    static T* Load(const String &filepath,
                   bool isEngineAsset = false)
    {
        T *a = nullptr;
        if (!AssetsManager::IsLoaded(filepath, isEngineAsset))
        {
            a = AssetsManager::ReadAssetFile<T>(filepath, isEngineAsset);
            AssetsManager::SaveAssetToMap(filepath, a, isEngineAsset); // Register it
        }
        else
        {
            a = AssetsManager::GetAsset<T>(filepath, isEngineAsset);
        }
        return a;
    }

    static void Unload(Asset *asset, bool isEngineAsset = false);
};

#endif // ASSETSMANAGER_H
