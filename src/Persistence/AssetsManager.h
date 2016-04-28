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

    //For every RELATIVE filepath, it contains a pointer to the asset
    // created when the assets were loaded.
    static std::map<std::string, Asset*> filepathToAssetPointer;

    //Filepath is parsed to RELATIVE, which is how cache works,
    //so you should try to use this method, to avoid using the
    //ABSOLUTE filepath instead of the RELATIVE one
    template <class T>
    static T* GetAssetFromMap(const std::string &filepath)
    {
        std::string f = Persistence::ProjectRootAbsoluteToRelative(filepath);

        if(!ExistsAssetInCache(f)) return nullptr;
        else return dynamic_cast<T*>(filepathToAssetPointer[f]);
    }

    //Reads an asset (*.btex2d, *.bmesh, etc.) from an stream
    //The input must not have the opening tag!
    template <class T>
    static T* ReadAsset(std::istream &f)
    {
        Asset *a = new T();
        FileReader::ReadNextLine(f); //Consume open tag
        a->Read(f);
        FileReader::ReadNextLine(f); //Consume close tag
        return dynamic_cast<T*>(a);
    }


    //Reads a specific asset file (*.btex2d, *.bmesh, etc.)
    // from a filepath
    template <class T>
    static T* ReadAssetFile(const std::string &filepath)
    {
        if(filepath != "-")
        {
            std::ifstream f (filepath);
            if ( !f.is_open() )
            {
                Logger_Error("Could not open the file '" << filepath <<
                             "' to load this asset.");
                return nullptr;
            }

            Asset *a = ReadAsset<T>(f);
            if(a != nullptr)
            {
                AssetsManager::SaveAsset(filepath, a);
                a->filepath = filepath;
                return dynamic_cast<T*>(a);
            }
        }

        return nullptr;
    }

    AssetsManager() {}

public:

    static bool ExistsAssetInCache(const std::string &filepath);

    //Tries to retrieve an Asset from the AssetsManager cache
    //If there's no such Asset, it is loaded, added to the cache, and returned
    //If the file can't be read, nullptr is returned
    template <class T>
    static T* GetAsset(const std::string &filepath)
    {
        Asset *a = nullptr;
        if(filepath != "-")
        {
            if(!ExistsAssetInCache(filepath))
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

        return a == nullptr ? nullptr : dynamic_cast<T*>(a);
    }

    //Tries to retrieve an Asset from the AssetsManager cache.
    //If there's no such Asset, nothing is loaded, it just returns nullptr.
    template <class T>
    static T* GetCachedAsset(const std::string &filepath)
    {
        Logger_Log("OSTIA");
        Logger_Log(filepathToAssetPointer);
        if(ExistsAssetInCache(filepath))
        {
            Logger_Log("Correcto");
        }
        return GetAssetFromMap<T>(filepath);
    }


    //Read an Asset, but you can later delete it, because it won't
    //use the AssetsManager cache, so nobody will be using the returned Asset
    template <class T>
    static T* ReadTmpAsset(const std::string &filepath)
    {
        std::ifstream is;
        is.open(filepath.c_str());
        if(!is.is_open()) return nullptr;
        T *a = ReadAsset<T>(is);
        is.close();
        return a;
    }

    static void SaveAsset(const std::string &filepath, Asset* pointerToAsset);
};

#endif // ASSETSMANAGER_H
