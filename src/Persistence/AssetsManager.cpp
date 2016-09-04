#include "AssetsManager.h"

#include "Scene.h"
#include "Application.h"
#include "SceneManager.h"

AssetsManager::AssetsManager()
{
}

AssetsManager::~AssetsManager()
{
    while (m_id_To_AssetPointer.Empty())
    {
        auto it = m_id_To_AssetPointer.Begin();
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
    if (!am) { return false; }

    String f = AssetsManager::FormatFilepath(filepath);
    return (am->m_id_To_AssetPointer.ContainsKey(f));
}

void AssetsManager::UnloadAsset(Asset *asset)
{
    AssetsManager *am = AssetsManager::GetCurrent(); NONULL(am);
    am->m_id_To_AssetPointer.RemoveValues(asset);
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
