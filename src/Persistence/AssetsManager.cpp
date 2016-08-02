#include "AssetsManager.h"

std::map<std::string, Asset*> AssetsManager::m_idToAssetPointer;

std::string AssetsManager::FormatFilepath(const std::string &filepath)
{
    return Persistence::ProjectRootAbsoluteToRelative(filepath);;
}

bool AssetsManager::IsAssetLoaded(const std::string &filepath)
{
    std::string f = AssetsManager::FormatFilepath(filepath);
    return (m_idToAssetPointer.find(f) != m_idToAssetPointer.end());
}

void AssetsManager::SaveAsset(const std::string &filepath, Asset *pointerToAsset)
{
    if (filepath != "")
    {
        std::string f = AssetsManager::FormatFilepath(filepath);
        m_idToAssetPointer[f] = pointerToAsset;
    }
}
