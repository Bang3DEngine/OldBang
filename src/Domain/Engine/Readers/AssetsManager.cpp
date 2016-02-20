#include "AssetsManager.h"

std::map<std::string, Asset*> AssetsManager::filepathToAssetPointer;

void AssetsManager::SaveAsset(const std::string &filepath, Asset *pointerToAsset)
{
    filepathToAssetPointer[filepath] = pointerToAsset;
}
