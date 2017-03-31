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

String AssetsManager::FormatFilepath(const String &filepath,
                                     bool isEngineAsset)
{
    if (IO::IsAbsolute(filepath))
    {   // Do nothing, maybe the user is looking for something outside of Assets
        return filepath;
    }
    return IO::GetEngineAssetsRootAbs() + "/" + filepath;
}

bool AssetsManager::IsLoaded(const String &filepath,
                             bool isEngineAsset)
{
    AssetsManager *am = AssetsManager::GetCurrent();
    if (!am) { return false; }

    String f = AssetsManager::FormatFilepath(filepath, isEngineAsset);
    return (am->m_id_To_AssetPointer.ContainsKey(f));
}

void AssetsManager::Unload(const String &id)
{
    AssetsManager *am = AssetsManager::GetCurrent(); ASSERT(am);
    am->m_id_To_AssetPointer.Remove(id);
}

void AssetsManager::Unload(Asset *asset)
{
    AssetsManager *am = AssetsManager::GetCurrent(); ASSERT(am);
    am->m_id_To_AssetPointer.RemoveValues(asset);
}

void AssetsManager::SaveAssetToMap(const String &filepath, Asset *asset, bool isEngineAsset)
{
    AssetsManager *am = AssetsManager::GetCurrent(); ASSERT(am);
    if (!filepath.Empty() && asset)
    {
        String f = FormatFilepath(filepath, isEngineAsset);
        if (!f.Empty())
        {
            am->m_id_To_AssetPointer[f] = asset;
        }
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
