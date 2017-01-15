#include "AssetsManager.h"

#include "Debug.h"
#include "Scene.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "Application.h"
#include "Persistence.h"
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

String AssetsManager::FormatFilepathForSearchingFile(const String &filepath,
                                                     bool isEngineAsset)
{
    if (Persistence::IsAbsolute(filepath))
    {   // Do nothing, maybe the user is looking for something outside of Assets
        return filepath;
    }
/*
    String prependDirectory = isEngineAsset ?
                Persistence::GetEngineAssetsRootAbs() :
                Persistence::GetProjectAssetsRootAbs();
*/
    return Persistence::ToAbsolute(filepath, isEngineAsset);
}

String AssetsManager::FormatFilepathForCacheMap(const String &filepath,
                                                bool isEngineAsset)
{
    String prependDirectory = isEngineAsset ?
                Persistence::GetEngineAssetsRootAbs() :
                Persistence::GetProjectAssetsRootAbs();

    // Temporal solution to avoid collisions of Engine assets with user assets
    //if (isEngineAsset)
    //    prependDirectory += "ENGINE";

    return Persistence::ToRelative(filepath);
}

bool AssetsManager::IsLoaded(const String &filepath,
                             bool isEngineAsset)
{
    AssetsManager *am = AssetsManager::GetCurrent();
    if (!am) { return false; }

    String f = AssetsManager::FormatFilepathForCacheMap(filepath, isEngineAsset);
    return (am->m_id_To_AssetPointer.ContainsKey(f));
}

void AssetsManager::Unload(Asset *asset, bool isEngineAsset)
{
    AssetsManager *am = AssetsManager::GetCurrent(); ASSERT(am);
    am->m_id_To_AssetPointer.RemoveValues(asset);
}

void AssetsManager::SaveAssetToMap(const String &filepath, Asset *asset, bool isEngineAsset)
{
    AssetsManager *am = AssetsManager::GetCurrent(); ASSERT(am);
    if (!filepath.Empty() && asset)
    {
        String f = FormatFilepathForCacheMap(filepath, isEngineAsset);
        if (!f.Empty())
        {
            am->m_id_To_AssetPointer[f] = asset;
        }
    }
}

void AssetsManager::SaveAssetToFile(const String &filepath, Asset *asset, bool isEngineAsset)
{
    if (!filepath.Empty() && asset)
    {
        String f = FormatFilepathForSearchingFile(filepath, isEngineAsset);
        FileWriter::WriteToFile(f, asset);
    }
}
