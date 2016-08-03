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

void AssetsManager::SaveAssetToMap(const std::string &filepath, Asset *pointerToAsset)
{
    if (filepath != "" && pointerToAsset)
    {
        std::string f = AssetsManager::FormatFilepath(filepath);
        m_idToAssetPointer[f] = pointerToAsset;
    }
}

void AssetsManager::SaveAssetToFile(const std::string &filepath, Asset *pointerToAsset)
{
    if (filepath != "" && pointerToAsset)
    {
        XMLNode *xmlInfo = new XMLNode();
        pointerToAsset->FillXMLInfo(xmlInfo);
        FileWriter::WriteToFile(filepath, xmlInfo->ToString());
        delete xmlInfo;
    }

}
