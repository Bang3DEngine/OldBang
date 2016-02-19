#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include <string>
#include <map>

#include "Asset.h"
#include "AssetsReader.h"

class AssetsManager
{

private:

    //For every filepath, it contains a pointer to the asset
    // created when the assets were loaded.
    static std::map<std::string, Asset*> filepathToAssetPointer;

    AssetsManager() {}

public:

    template <class T>
    static T* GetAsset(const std::string &filepath)
    {
        Logger_Log("Get Asset " << filepath);
        Asset *a = nullptr;
        if(filepathToAssetPointer.find(filepath) == filepathToAssetPointer.end())
        {
            Logger_Log("ReadAssetFile called");
            //Doesnt have the Asset created. Create, read, and save it
            a = AssetsReader::ReadAssetFile<T>(filepath);
            SaveAsset(filepath, a);
        }
        else
        {
            Logger_Log("Directly retrieved");
            a = filepathToAssetPointer[filepath];
        }

        return a == nullptr ? nullptr : dynamic_cast<T*>(a);
    }

    static void SaveAsset(const std::string &filepath, Asset* pointerToAsset);
};

#endif // ASSETSMANAGER_H
