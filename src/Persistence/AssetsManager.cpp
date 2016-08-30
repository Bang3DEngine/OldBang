#include "AssetsManager.h"

#include "Scene.h"
#include "SceneManager.h"

AssetsManager::AssetsManager()
{
}

AssetsManager::~AssetsManager()
{
    while (m_idToAssetPointer.empty())
    {
        auto it = m_idToAssetPointer.begin();
        delete it->second;
    }
}

AssetsManager *AssetsManager::GetCurrent()
{
    Scene *scene = SceneManager::GetActiveScene();
    return  scene ? scene->GetAssetsManager() : nullptr;
}

String AssetsManager::FormatFilepath(const String &filepath)
{
    return Persistence::ToRelative(filepath);;
}

bool AssetsManager::IsAssetLoaded(const String &filepath)
{
    AssetsManager *am = AssetsManager::GetCurrent();
    if (!am)
    {
        return false;
    }

    String f = AssetsManager::FormatFilepath(filepath);
    return (am->m_idToAssetPointer.find(f) != am->m_idToAssetPointer.end());
}

void AssetsManager::UnloadAsset(const Asset *asset)
{
    AssetsManager *am = AssetsManager::GetCurrent(); NONULL(am);
    for (auto it = am->m_idToAssetPointer.begin();
         it != am->m_idToAssetPointer.end(); ++it)
    {
        if (((void*)(it->second)) == ((void*)(asset)))
        {
            am->m_idToAssetPointer.erase(it);
            break;
        }
    }
}

void AssetsManager::SaveAssetToMap(const String &filepath, Asset *asset)
{
    AssetsManager *am = AssetsManager::GetCurrent(); NONULL(am);
    if (!filepath.Empty() && asset)
    {
        String f = FormatFilepath(filepath);
        if (!f.Empty())
        {
            am->m_idToAssetPointer[f] = asset;
        }
    }
}

void AssetsManager::SaveAssetToFile(const String &filepath, Asset *asset)
{
    if (filepath != "" && asset)
    {
        FileWriter::WriteToFile(filepath, asset);
    }
}
