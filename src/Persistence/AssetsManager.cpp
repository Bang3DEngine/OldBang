#include "AssetsManager.h"

std::map<std::string, Asset*> AssetsManager::filepathToAssetPointer;

bool AssetsManager::ExistsAssetInCache(const std::string &filepath)
{
    std::string f = Persistence::ProjectRootAbsoluteToRelative(filepath);
    return (filepathToAssetPointer.find(f) !=
            filepathToAssetPointer.end());
}

void AssetsManager::SaveAsset(const std::string &filepath, Asset *pointerToAsset)
{
    std::string f = Persistence::ProjectRootAbsoluteToRelative(filepath);
    filepathToAssetPointer[filepath] = pointerToAsset;
}
