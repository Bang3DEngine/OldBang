#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include <sstream>

#include "Map.h"
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

    static String FormatFilepath(const String &filepath);

    template <class T>
    static T* GetAsset(const String &filepath)
    {
        AssetsManager *am = AssetsManager::GetCurrent();
        if (!AssetsManager::IsAssetLoaded(filepath)) return nullptr;
        else
        {
            String f = AssetsManager::FormatFilepath(filepath);
            return dynamic_cast<T*>(am->m_id_To_AssetPointer[f]);
        }
    }

    /** Reads a specific asset file (btex2d, bmesh, etc.)
      * from a filepath.
    **/
    template <class T>
    static T* ReadAssetFile(const String &filepath)
    {
        T *a = nullptr;
        XMLNode *xmlInfo = XMLParser::FromFile(filepath);
        if (xmlInfo)
        {
            a = new T();
            a->ReadXMLInfo(xmlInfo);
            a->m_filepath = filepath;
            delete xmlInfo;
        }
        return a;
    }

    static void SaveAssetToMap(const String &filepath, Asset* asset);
    static void SaveAssetToFile(const String &filepath, Asset* asset);

    static AssetsManager* GetCurrent();

public:

    AssetsManager();
    virtual ~AssetsManager();

    static bool IsAssetLoaded(const String &filepath);

    /** Tries to retrieve an Asset from the AssetsManager cache
      * If there's no such Asset, it is loaded, added to the cache, and returned
      * If the file can't be read, nullptr is returned  **/
    template <class T>
    static T* CreateAsset(const String &filepath)
    {
        T *a = new T();
        //a->m_filepath = filepath;
        AssetsManager::SaveAssetToFile(filepath, a);
        return a;
    }

    /** Tries to retrieve an Asset from the AssetsManager cache
      * If there's no such Asset, it is loaded, added to the cache, and returned
      * If the file can't be read, nullptr is returned  **/
    template <class T>
    static T* LoadAsset(const String &filepath)
    {
        T *a = nullptr;
        if (!AssetsManager::IsAssetLoaded(filepath))
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

    static void UnloadAsset(Asset *asset);
};

#endif // ASSETSMANAGER_H
