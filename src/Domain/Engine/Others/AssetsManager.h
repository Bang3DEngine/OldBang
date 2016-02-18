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

    AssetsManager() {}

public:

    template <class T>
    static T* GetAsset(const std::string &filepath)
    {
        if(filepathToAssetPointer.find(filepath) == filepathToAssetPointer.end())
            return nullptr;
        return dynamic_cast<T*>(filepathToAssetPointer[filepath]);
    }

    static void SaveAsset(const std::string &filepath, Asset* pointerToAsset);
};

#endif // ASSETSMANAGER_H
