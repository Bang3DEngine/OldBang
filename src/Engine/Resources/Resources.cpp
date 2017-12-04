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
    for (IResource *res : resources) { Resources::Unload(res); }
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

void Resources::Unload(const GUID &guid)
{
    Resources *rs = Resources::GetInstance();
    if (rs)
    {
        for (auto& itTypeMap : rs->m_GUIDToResource)
        {
            for (auto& itGUIDRes : itTypeMap.second)
            {
                const GUID &foundGUID = itGUIDRes.first;
                if (foundGUID == guid)  { UnloadSingleResource(itGUIDRes.second);  }
            }
            (itTypeMap.second).Remove(guid);
        }
    }
}

void Resources::Unload(IResource *res)
{
    Resources *rs = Resources::GetInstance();
    if (rs)
    {
        for (auto& itTypeMap : rs->m_GUIDToResource)
        {
            for (const auto& itGUIDRes : itTypeMap.second)
            {
                IResource *foundRes = itGUIDRes.second;
                if (foundRes == res ) { UnloadSingleResource(res); }
            }
            (itTypeMap.second).RemoveValues(res);
        }
    }
    else { UnloadSingleResource(res); }
}

void Resources::UnloadSingleResource(IResource *res)
{
    Resources *rs = Resources::GetInstance();

    bool destroy = true;
    if (rs)
    {
        ASSERT(rs->m_resourcesUsage.ContainsKey(res));
        uint *resourcesUsage = &(rs->m_resourcesUsage[res]);

        --(*resourcesUsage);
        ASSERT(*resourcesUsage >= 0);

        destroy = (*resourcesUsage == 0);
    }

    if (destroy) { Destroy(res); }
}

#ifdef DEBUG
bool Resources::_AssertCreatedFromResources = false;
bool Resources::_AssertDestroyedFromResources = false;

bool Resources::AssertCreatedFromResources()
{
    return !Resources::GetInstance() || Resources::_AssertCreatedFromResources;
}

bool Resources::AssertDestroyedFromResources()
{
    return !Resources::GetInstance() || Resources::_AssertDestroyedFromResources;
}
#endif

void Resources::Destroy(IResource *resource)
{
    #ifdef DEBUG
    Resources::_AssertDestroyedFromResources = true;
    #endif

    Asset *asset = Cast<Asset*>(resource);
    if (asset) { Asset::Destroy(asset); } else { delete resource; }

    #ifdef DEBUG
    Resources::_AssertDestroyedFromResources = false;
    #endif
}

Resources *Resources::GetInstance()
{
    Window *win = Window::GetActive();
    return  win ? win->GetResources() : nullptr;
}
