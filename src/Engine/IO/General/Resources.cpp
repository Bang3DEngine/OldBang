#include "Bang/Resources.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Application.h"

Resources::Resources()
{
}

Resources::~Resources()
{
    List<Resource*> resources = m_idToResource.GetValues();
    for (Resource *res : resources) { delete res; }
}

void Resources::Add(const String &id, Resource *x)
{
    Resources *am = Resources::GetInstance();
    if (!id.IsEmpty()) { am->m_idToResource.Add(id, x); }
}

Resources *Resources::GetInstance()
{
    Application *app = Application::GetInstance();
    return  app ? app->GetResources() : nullptr;
}

Resource *Resources::Get(const String &id)
{
    Resources *ress = Resources::GetInstance();
    if (Resources::Contains(id)) { return ress->m_idToResource.Get(id); }
    return nullptr;
}

bool Resources::Contains(const String &id)
{
    Resources *am = Resources::GetInstance();
    return am->m_idToResource.ContainsKey(id);
}

void Resources::UnLoad(const String &id, bool deleteResource)
{
    Resources *rs = Resources::GetInstance();
    Resource *res = rs->m_idToResource.Get(id);
    if (deleteResource) { delete res; }
    rs->m_idToResource.Remove(id);
}

void Resources::UnLoad(Resource *res, bool deleteResource)
{
    Resources *rs = Resources::GetInstance();
    if (deleteResource)
    {
        for (const auto& itPair : rs->m_idToResource)
        {
            if ( itPair.second == res) { delete res; }
        }
    }
    rs->m_idToResource.RemoveValues(res);

}
