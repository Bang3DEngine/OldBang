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

void Resources::Unload(const String &id)
{
    Resources *am = Resources::GetInstance();
    am->m_idToResource.Remove(id);
}

void Resources::Unload(Resource *res)
{
    Resources *ress = Resources::GetInstance();
    ress->m_idToResource.RemoveValues(res);

}
