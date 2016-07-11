#include "AssetsManager.h"

std::map<std::string, Asset*> AssetsManager::m_idToAssetPointer;

bool AssetsManager::ExistsAssetInCache(const std::string &id)
{
    std::string f = Persistence::ProjectRootAbsoluteToRelative(id);
    return (m_idToAssetPointer.find(f) != m_idToAssetPointer.end());
}

void AssetsManager::SaveRuntimeAsset(const std::string &uniqueId,
                                     Asset *pointerToAsset)
{
    SaveAsset(uniqueId, pointerToAsset);
}

void AssetsManager::SaveAsset(const std::string &filepath,
                              Asset *pointerToAsset) {
    std::string f = Persistence::ProjectRootAbsoluteToRelative(filepath);
    m_idToAssetPointer[filepath] = pointerToAsset;
}
