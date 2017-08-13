#include "Bang/AssetsManager.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Application.h"

AssetsManager::AssetsManager()
{
}

AssetsManager::~AssetsManager()
{
    List<Asset*> assets = m_idToAsset.GetValues();
    for (Asset *asset : assets) { delete asset; }
}

AssetsManager *AssetsManager::GetInstance()
{
    Application *app = Application::GetInstance();
    return  app ? app->GetAssetsManager() : nullptr;
}

Asset *AssetsManager::Get(const String &id)
{
    AssetsManager *am = AssetsManager::GetInstance();
    if (AssetsManager::Contains(id)) { return am->m_idToAsset.Get(id); }
    return nullptr;
}

bool AssetsManager::Contains(const String &id)
{
    AssetsManager *am = AssetsManager::GetInstance();
    return am->m_idToAsset.ContainsKey(id);
}

void AssetsManager::Unload(const String &id)
{
    AssetsManager *am = AssetsManager::GetInstance();
    am->m_idToAsset.Remove(id);
}

void AssetsManager::Unload(Asset *asset)
{
    AssetsManager *am = AssetsManager::GetInstance();
    am->m_idToAsset.RemoveValues(asset);

}
