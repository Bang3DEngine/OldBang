#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include "Bang.h"

#include <map>
#include <string>
#include <sstream>

#include "Asset.h"
#include "Persistence.h"

class AssetsManager
{
private:

    /** For every id, it contains a pointer to the asset
      * created when the assets were loaded.
    **/
    static std::map<std::string, Asset*> m_idToAssetPointer;

    static std::string FormatFilepath(const std::string &filepath);

    template <class T>
    static T* GetAsset(const std::string &filepath)
    {
        if (!IsAssetLoaded(filepath)) return nullptr;
        else
        {
            std::string f = AssetsManager::FormatFilepath(filepath);
            return dynamic_cast<T*>(m_idToAssetPointer[f]);
        }
    }

    /** Reads a specific asset file (btex2d, bmesh, etc.)
      * from a filepath.
    **/
    template <class T>
    static T* ReadAssetFile(const std::string &filepath)
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

    static void SaveAssetToMap(const std::string &filepath, Asset* pointerToAsset);
    static void SaveAssetToFile(const std::string &filepath, Asset* pointerToAsset);

    AssetsManager() {}

public:

    static bool IsAssetLoaded(const std::string &filepath);

    /** Tries to retrieve an Asset from the AssetsManager cache
      * If there's no such Asset, it is loaded, added to the cache, and returned
      * If the file can't be read, nullptr is returned  **/
    template <class T>
    static T* CreateAsset(const std::string &filepath)
    {
        T *a = new T();
        //a->m_filepath = filepath;
        SaveAssetToFile(filepath, a);
        return a;
    }

    /** Tries to retrieve an Asset from the AssetsManager cache
      * If there's no such Asset, it is loaded, added to the cache, and returned
      * If the file can't be read, nullptr is returned  **/
    template <class T>
    static T* LoadAsset(const std::string &filepath)
    {
        T *a = nullptr;
        if (!IsAssetLoaded(filepath))
        {
            a = ReadAssetFile<T>(filepath);
            SaveAssetToMap(filepath, a);
        }
        else
        {
            a = GetAsset<T>(filepath);
        }
        return a;
    }
};

#endif // ASSETSMANAGER_H
