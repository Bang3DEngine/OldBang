#include "Bang/Resources.h"

#include "Bang/File.h"
#include "Bang/Asset.h"
#include "Bang/Paths.h"
#include "Bang/Object.h"
#include "Bang/Random.h"
#include "Bang/Window.h"
#include "Bang/ObjectManager.h"

USING_NAMESPACE_BANG

Resources::Resources()
{
}

Resources::~Resources()
{
    Array<IResource*> resources = Resources::GetAllResources();
    for (IResource *res : resources) { Destroy(res); }
}

Array<IResource*> Resources::GetAllResources()
{
    Array<IResource*> result;
    Resources *rs = Resources::GetInstance();
    for (const auto& itTypeMap : rs->m_GUIDToResource)
    {
        result.PushBack(itTypeMap.second.GetValues());
    }
    return result;
}

void Resources::UnLoad(const GUID &guid, bool deleteResource)
{
    Resources *res = Resources::GetInstance();
    for (auto& itTypeMap : res->m_GUIDToResource)
    {
        if (deleteResource)
        {
            for (auto& itGUIDRes : itTypeMap.second)
            {
                if (itGUIDRes.first == guid) { Destroy(itGUIDRes.second); }
            }
        }
        (itTypeMap.second).Remove(guid);
    }
}

void Resources::Destroy(IResource *resource)
{
    Asset *asset = Cast<Asset*>(resource);
    if (asset) {  Asset::Destroy(asset); } else { delete resource; }
}

void Resources::UnLoad(IResource *res, bool deleteResource)
{
    Resources *rs = Resources::GetInstance();
    for (auto& itTypeMap : rs->m_GUIDToResource)
    {
        if (deleteResource)
        {
            for (const auto& itGUIDRes : itTypeMap.second)
            {
                if (itGUIDRes.second == res ) { Destroy(res); }
            }
        }
        (itTypeMap.second).RemoveValues(res);
    }
}

Resources *Resources::GetInstance()
{
    Window *win = Window::GetActive();
    return  win ? win->GetResources() : nullptr;
}
