#include "AssetsManager.h"

#include "Scene.h"
#include "Application.h"
#include "SceneManager.h"

AssetsManager::AssetsManager()
{
}

AssetsManager::~AssetsManager()
{
    while (m_id_To_AssetPointer.empty())
    {
        auto it = m_id_To_AssetPointer.begin();
        delete it->second;
    }
}

AssetsManager *AssetsManager::GetCurrent()
{
    Application *app = Application::GetInstance();
    return  app ? app->GetAssetsManager() : nullptr;
}

String AssetsManager::FormatFilepath(const String &filepath)
{
    return Persistence::ToRelative(filepath);
}

bool AssetsManager::IsAssetLoaded(const String &filepath)
{
    AssetsManager *am = AssetsManager::GetCurrent();
    if (!am)
    {
        return false;
    }

    String f = AssetsManager::FormatFilepath(filepath);
    return (am->m_id_To_AssetPointer.find(f) != am->m_id_To_AssetPointer.end());
}

void AssetsManager::UnloadAsset(const Asset *asset)
{
    AssetsManager *am = AssetsManager::GetCurrent(); NONULL(am);
    for (auto it = am->m_id_To_AssetPointer.begin();
         it != am->m_id_To_AssetPointer.end(); ++it)
    {
        if (((void*)(it->second)) == ((void*)(asset)))
        {
            am->m_id_To_AssetPointer.erase(it);
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
            am->m_id_To_AssetPointer[f] = asset;
        }
    }
}

void AssetsManager::SaveAssetToFile(const String &filepath, Asset *asset)
{
    if (!filepath.Empty() && asset)
    {
        FileWriter::WriteToFile(filepath, asset);
    }
}
