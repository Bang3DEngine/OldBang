#include "Bang/Resources.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Random.h"
#include "Bang/Application.h"

Resources::Resources()
{
}

Resources::~Resources()
{
    List<Resource*> resources = m_GUIDToResource.GetValues();
    for (Resource *res : resources) { delete res; }
}

void Resources::Add(const GUID &guid, Resource *x)
{
    Resources *am = Resources::GetInstance();
    if (!guid.IsEmpty()) { am->m_GUIDToResource.Add(guid, x); }
}

Resources *Resources::GetInstance()
{
    Application *app = Application::GetInstance();
    return  app ? app->GetResources() : nullptr;
}

Resource *Resources::GetCached(const GUID &guid)
{
    Resources *ress = Resources::GetInstance();
    if (Resources::Contains(guid)) { return ress->m_GUIDToResource.Get(guid); }
    return nullptr;
}

bool Resources::Contains(const GUID &guid)
{
    if (guid.IsEmpty()) { return false; }

    Resources *am = Resources::GetInstance();
    return am->m_GUIDToResource.ContainsKey(guid);
}

void Resources::UnLoad(const GUID &guid, bool deleteResource)
{
    Resources *rs = Resources::GetInstance();
    Resource *res = rs->m_GUIDToResource.Get(guid);
    if (deleteResource) { delete res; }
    rs->m_GUIDToResource.Remove(guid);
}

void Resources::UnLoad(Resource *res, bool deleteResource)
{
    Resources *rs = Resources::GetInstance();
    if (deleteResource)
    {
        for (const auto& itPair : rs->m_GUIDToResource)
        {
            if ( itPair.second == res) { delete res; }
        }
    }
    rs->m_GUIDToResource.RemoveValues(res);

}


