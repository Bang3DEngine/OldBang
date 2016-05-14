#include "AssetsManager.h"

std::map<std::string, Asset*> AssetsManager::idToAssetPointer;

bool AssetsManager::ExistsAssetInCache(const std::string &id)
{
    std::string f = Persistence::ProjectRootAbsoluteToRelative(id);
    return (idToAssetPointer.find(f) != idToAssetPointer.end());
}

void AssetsManager::SaveRuntimeAsset(const std::string &uniqueId,
                                     Asset *pointerToAsset)
{
    SaveAsset(uniqueId, pointerToAsset);
}

void AssetsManager::SaveAsset(const std::string &filepath,
                              Asset *pointerToAsset) {
    std::string f = Persistence::ProjectRootAbsoluteToRelative(filepath);
    idToAssetPointer[filepath] = pointerToAsset;
}
