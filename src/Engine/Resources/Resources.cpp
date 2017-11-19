#include "Bang/Resources.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Random.h"
#include "Bang/Window.h"

USING_NAMESPACE_BANG

Resources::Resources()
{
}

Resources::~Resources()
{
    Array<Resource*> resources = Resources::GetAllResources();
    for (Resource *res : resources) { delete res; }
}

Array<Resource *> Resources::GetAllResources()
{
    Array<Resource*> result;
    Resources *rs = Resources::GetInstance();
    for (const auto& itTypeMap : rs->m_GUIDToResource)
    {
        result.PushBack(itTypeMap.second.GetValues());
    }
    return result;
}

void Resources::UnLoad(const GUID &guid, bool deleteResource)
{
    Resources *imp = Resources::GetInstance();
    for (auto& itTypeMap : imp->m_GUIDToResource)
    {
        if (deleteResource)
        {
            for (const auto& itGUIDRes : itTypeMap.second)
            {
                if (itGUIDRes.first == guid) { delete itGUIDRes.second; }
            }
        }
        (itTypeMap.second).Remove(guid);
    }
}

void Resources::UnLoad(Resource *res, bool deleteResource)
{
    Resources *rs = Resources::GetInstance();
    for (auto& itTypeMap : rs->m_GUIDToResource)
    {
        if (deleteResource)
        {
            for (const auto& itGUIDRes : itTypeMap.second)
            {
                if (itGUIDRes.second == res ) { delete res; }
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
