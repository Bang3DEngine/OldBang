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
    while (!m_idToAssetPointer.Empty())
    {
        auto it = m_idToAssetPointer.Begin();
        delete it->second;
    }
}

AssetsManager *AssetsManager::GetCurrent()
{
    Application *app = Application::GetInstance();
    return  app ? app->GetAssetsManager() : nullptr;
}

String AssetsManager::FormatFilepath(const String &filepath,
                                     bool isEngineAsset)
{
    return IO::ToAbsolute(filepath, isEngineAsset);
}

Asset *AssetsManager::GetAsset(const String &filepath, bool isEngineAsset)
{
    AssetsManager *am = AssetsManager::GetCurrent();
    if (AssetsManager::IsLoaded(filepath, isEngineAsset))
    {
        String f = AssetsManager::FormatFilepath(filepath, isEngineAsset);
        return am->m_idToAssetPointer.Get(f);
    }
    return nullptr;
}

#ifdef BANG_EDITOR

void AssetsManager::UpdateAsset(const String &assetFilepath,
                                const XMLNode &xmlChangedInfo)
{
    // Update live instances and rewrite the file
    ENSURE(!assetFilepath.Empty());
    if (AssetsManager::IsLoaded(assetFilepath, false))
    {
        Asset *asset = AssetsManager::GetAsset(assetFilepath, false);
        asset->Read(xmlChangedInfo);
    }

    Debug_Log("UpdateAsset " << assetFilepath);
    if (IO::ExistsFile(assetFilepath))
    {
        IO::WriteToFile(assetFilepath, xmlChangedInfo.ToString());
    }
}

void AssetsManager::InvalidateAsset(const String &assetFilepath)
{
    AssetsManager *am = AssetsManager::GetCurrent();
    String f = AssetsManager::FormatFilepath(assetFilepath, false);
    am->m_idToAssetPointer.Remove(f);
}

void AssetsManager::ReloadAsset(const String &assetFilepath)
{
    ENSURE(!assetFilepath.Empty());
    Asset *asset = AssetsManager::GetAsset(assetFilepath, false);
    if (asset)
    {
        asset->ReadFromFile(assetFilepath);
    }
}

#endif

bool AssetsManager::IsLoaded(const String &filepath,
                             bool isEngineAsset)
{
    AssetsManager *am = AssetsManager::GetCurrent();
    if (!am) { return false; }

    String f = AssetsManager::FormatFilepath(filepath, isEngineAsset);
    return (am->m_idToAssetPointer.ContainsKey(f));
}

void AssetsManager::Unload(const String &id)
{
    AssetsManager *am = AssetsManager::GetCurrent(); ENSURE(am);
    String f = AssetsManager::FormatFilepath(id, false);
    am->m_idToAssetPointer.Remove(f);
}

void AssetsManager::Unload(Asset *asset)
{
    AssetsManager *am = AssetsManager::GetCurrent(); ENSURE(am);
    am->m_idToAssetPointer.RemoveValues(asset);
}

void AssetsManager::SaveAssetToMap(const String &filepath,
                                   Asset *asset,
                                   bool isEngineAsset)
{
    AssetsManager *am = AssetsManager::GetCurrent(); ENSURE(am);
    if (!filepath.Empty() && asset)
    {
        String f = FormatFilepath(filepath, isEngineAsset);
        am->m_idToAssetPointer[f] = asset;
    }
}

void AssetsManager::SaveAssetToFile(const String &filepath,
                                    Asset *asset,
                                    bool isEngineAsset)
{
    if (!filepath.Empty() && asset)
    {
        String f = FormatFilepath(filepath, isEngineAsset);
        IO::WriteToFile(f, asset->GetSerializedString());
    }
}
