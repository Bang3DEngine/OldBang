#include "Stage.h"


Stage::Stage()
{
}

Stage::~Stage()
{
    for(auto e = entities.begin(); e != entities.end(); ++e)
    {
        Entity *ent = *e;
        ent->_OnDestroy();
        delete ent;
    }
}

Entity *Stage::AddEntity(const std::string &name)
{
    Entity *ent =  new Entity();
    ent->name = name;
    entities.push_back(ent);
    return ent;
}

Entity *Stage::GetEntity(const std::string &name) const
{
    for(auto it = entities.begin(); it != entities.end(); ++it)
    {
        if((*it)->name == name)
        {
            return (*it);
        }
    }
    return nullptr;
}

void Stage::_OnStart()
{
    OnStart();
    PROPAGATE_EVENT(_OnStart, entities);
}

void Stage::_OnUpdate()
{
    OnUpdate();
    PROPAGATE_EVENT(_OnUpdate, entities);
}

void Stage::_OnRender()
{
    OnRender();
    PROPAGATE_EVENT(_OnRender, entities);
}


void Stage::_OnDestroy()
{
    OnDestroy();
    PROPAGATE_EVENT(_OnDestroy, entities);
}


