#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include <string>
#include <map>

#include "Asset.h"

class AssetsManager
{

private:

    //For every filepath, it contains a pointer to the asset
    // created when the assets were loaded.
    static std::map<std::string, Asset*> filepathToAssetPointer;

    //The input must not have the opening tag!
    template <class T>
    static T* ReadAsset(std::ifstream &f)
    {
        Asset *a = new T();
        L("Open tag consume..."); FileReader::ReadNextLine(f); //Consume open tag
        a->Read(f);
        L("Close tag consume..."); FileReader::ReadNextLine(f); //Consume close tag
        return dynamic_cast<T*>(a);
    }


    //Reads a specific asset file (*.asset)
    template <class T>
    static T* ReadAssetFile(const std::string &filepath)
    {
        L("ReadAssetFile...");
        std::ifstream f (filepath);
        if ( !f.is_open() )
        {
            Logger_Error("Could not open the file '" << filepath << "' to load this asset.");
            return nullptr;
        }

        Asset *a = ReadAsset<T>(f);
        AssetsManager::SaveAsset(a->GetFilepath(), a);

        return dynamic_cast<T*>(a);
    }

    AssetsManager() {}

public:

    template <class T>
    static T* GetAsset(const std::string &filepath)
    {
        Asset *a = nullptr;
        if(filepathToAssetPointer.find(filepath) == filepathToAssetPointer.end())
        {
            //Doesnt have the Asset created. Create, read, and save it
            a = ReadAssetFile<T>(filepath);
            SaveAsset(filepath, a);
        }
        else
        {
            a = filepathToAssetPointer[filepath];
        }

        return a == nullptr ? nullptr : dynamic_cast<T*>(a);
    }

    static void SaveAsset(const std::string &filepath, Asset* pointerToAsset);
};

#endif // ASSETSMANAGER_H
