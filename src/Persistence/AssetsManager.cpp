#include "Bang/AssetsManager.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/FileReader.h"
#include "Bang/Application.h"
#include "Bang/IO.h"
#include "Bang/SceneManager.h"

AssetsManager::AssetsManager()
{
}

AssetsManager::~AssetsManager()
{
    while (!m_pathsToAssets.Empty())
    {
        auto it = m_pathsToAssets.Begin();
        delete it->second;
    }
}

AssetsManager *AssetsManager::GetCurrent()
{
    Application *app = Application::GetInstance();
    return  app ? app->GetAssetsManager() : nullptr;
}

Asset *AssetsManager::GetAsset(const Path &filepath)
{
    AssetsManager *am = AssetsManager::GetCurrent();
    if (AssetsManager::IsLoaded(filepath))
    {
        return am->m_pathsToAssets.Get(filepath);
    }
    return nullptr;
}

#ifdef BANG_EDITOR

void AssetsManager::UpdateAsset(const Path &assetFilepath,
                                const XMLNode &xmlChangedInfo)
{
    // Update live instances and rewrite the file
    ENSURE(!assetFilepath.IsEmpty());
    if (AssetsManager::IsLoaded(assetFilepath))
    {
        Asset *asset = AssetsManager::GetAsset(assetFilepath);
        asset->Read(xmlChangedInfo);
    }

    if (assetFilepath.Exists())
    {
        IO::WriteToFile(assetFilepath.GetAbsolute(), xmlChangedInfo.ToString());
    }
}

void AssetsManager::InvalidateAsset(const Path &assetFilepath)
{
    AssetsManager *am = AssetsManager::GetCurrent();
    am->m_pathsToAssets.Remove(assetFilepath);
}

void AssetsManager::ReloadAsset(const Path &assetFilepath)
{
    ENSURE(!assetFilepath.IsEmpty());
    Asset *asset = AssetsManager::GetAsset(assetFilepath);
    if (asset) { asset->ReadFromFile(assetFilepath); }
}

#endif

bool AssetsManager::IsLoaded(const Path &filepath)
{
    AssetsManager *am = AssetsManager::GetCurrent();
    if (!am) { return false; }

    return (am->m_pathsToAssets.ContainsKey(filepath));
}

void AssetsManager::Unload(const Path &filepath)
{
    AssetsManager *am = AssetsManager::GetCurrent(); ENSURE(am);
    am->m_pathsToAssets.Remove(filepath);
}

void AssetsManager::Unload(Asset *asset)
{
    AssetsManager *am = AssetsManager::GetCurrent(); ENSURE(am);
    am->m_pathsToAssets.RemoveValues(asset);
}

void AssetsManager::SaveAssetToMap(const Path &filepath, Asset *asset)
{
    AssetsManager *am = AssetsManager::GetCurrent(); ENSURE(am);
    if (!filepath.IsEmpty() && asset)
    {
        am->m_pathsToAssets.Set(filepath, asset);
    }
}

void AssetsManager::SaveAssetToFile(const Path &filepath, Asset *asset)
{
    if (!filepath.IsEmpty() && asset)
    {
        IO::WriteToFile(filepath.GetAbsolute(), asset->GetSerializedString());
    }
}
