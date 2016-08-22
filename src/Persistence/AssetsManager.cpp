#include "AssetsManager.h"

std::map<String, Asset*> AssetsManager::m_idToAssetPointer;

String AssetsManager::FormatFilepath(const String &filepath)
{
    return Persistence::ToRelative(filepath);;
}

bool AssetsManager::IsAssetLoaded(const String &filepath)
{
    String f = AssetsManager::FormatFilepath(filepath);
    return (m_idToAssetPointer.find(f) != m_idToAssetPointer.end());
}

void AssetsManager::SaveAssetToMap(const String &filepath, Asset *pointerToAsset)
{
    if (filepath != "" && pointerToAsset)
    {
        String f = AssetsManager::FormatFilepath(filepath);
        m_idToAssetPointer[f] = pointerToAsset;
    }
}

void AssetsManager::SaveAssetToFile(const String &filepath, Asset *pointerToAsset)
{
    if (filepath != "" && pointerToAsset)
    {
        FileWriter::WriteToFile(filepath, pointerToAsset);
    }

}
