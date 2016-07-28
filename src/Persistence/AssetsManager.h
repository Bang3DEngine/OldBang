#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include "Bang.h"

#include <string>
#include <map>

#include "Asset.h"
#include "Persistence.h"

class AssetsManager

{

private:
    /** For every RELATIVE id, it contains a pointer to the asset
      * created when the assets were loaded.
    **/
    static std::map<std::string, Asset*> m_idToAssetPointer;

    /** Filepath is parsed to RELATIVE, which is how cache works,
      * so you should try to use this method, to avoid using the
      * ABSOLUTE filepath instead of the RELATIVE one.
    **/
    template <class T>
    static T* GetAssetFromMap(const std::string &filepath)
    {
        std::string f = Persistence::ProjectRootAbsoluteToRelative(filepath);

        if (!ExistsAssetInCache(f)) return nullptr;
        else return dynamic_cast<T*>(m_idToAssetPointer[f]);
    }

    /** Reads an asset (*.btex2d, *.bmesh, etc.) from an stream
      * The input must not have the opening tag!.
    **/
    template <class T>
    static T* ReadAsset(const std::string &fileContents)
    {
        Asset *a = new T();
        //Logger_Log("Reading asset: \n" << fileContents);
        XMLNode *xmlInfo = XMLParser::FromXML(fileContents);
        a->ReadXMLInfo(xmlInfo);
        delete xmlInfo;
        return static_cast<T*>(a);
    }

    /** Reads a specific asset file (*.btex2d, *.bmesh, etc.)
      * from a filepath.
    **/
    template <class T>
    static T* ReadAssetFile(const std::string &filepath)
    {
        XMLNode *xmlInfo = XMLParser::FromFile(filepath);
        Asset *a = ReadAsset<T>(xmlInfo->ToString());
        delete xmlInfo;
        if (a)
        {
            AssetsManager::SaveAsset(filepath, a);
            a->m_filepath = filepath;
            return static_cast<T*>(a);
        }
        return nullptr;
    }

    AssetsManager() {}

public:

    static bool ExistsAssetInCache(const std::string &filepath);

    /** Tries to retrieve an Asset from the AssetsManager cache.
      * It won't try to read any file. If it fails, it just returns nullptr **/
    template <class T>
    static T* GetRuntimeAsset(const std::string &id)
    {
        return GetAssetFromMap<T>(id);
    }

    /** Tries to retrieve an Asset from the AssetsManager cache
      * If there's no such Asset, it is loaded, added to the cache, and returned
      * If the file can't be read, nullptr is returned  **/
    template <class T>
    static T* GetAsset(const std::string &filepath)
    {
        Asset *a = nullptr;
        //Logger_Log("Getting asset: " << filepath);
        if (filepath != "-")
        {
            if (!ExistsAssetInCache(filepath))
            {
                //It doesnt have the Asset loaded. Read, and save it to cache.
                a = ReadAssetFile<T>(filepath);
                SaveAsset(filepath, a);
            }
            else
            {
                a = GetCachedAsset<T>(filepath);
            }
        }

        return a ? static_cast<T*>(a) : nullptr;
    }

    /** Tries to retrieve an Asset from the AssetsManager cache.
      * If there's no such Asset, nothing is loaded, it just returns nullptr. **/
    template <class T>
    static T* GetCachedAsset(const std::string &id)
    {
        return GetAssetFromMap<T>(id);
    }

    /** Read an Asset, but you can later delete it, because it won't
      * use the AssetsManager cache, so nobody will be using the returned Asset. **/
    template <class T>
    static T* ReadTmpAsset(const std::string &filepath)
    {
        std::ifstream f;
        f.open(filepath.c_str());
        if (!f.is_open()) return nullptr;

        std::string contents((std::istreambuf_iterator<char>(f)),
                              std::istreambuf_iterator<char>());
        T *a = ReadAsset<T>(contents);
        f.close();
        return a;
    }

    static void SaveRuntimeAsset(const std::string &uniqueId, Asset* pointerToAsset);
    static void SaveAsset(const std::string &filepath, Asset* pointerToAsset);
};

#endif // ASSETSMANAGER_H
